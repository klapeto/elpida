/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 15/9/2024.
//
// Modifications:
//  - optimization: use std::sting_view instead of std::string for base64_chars. encoding throughput increased by ~14%
//  - optimization: use a local reference of base64_chars on stack
//  - optimization: Inlined functions
//  - optimization: reserve the memory needed by an approximation of what needed
//  - decoding: look the characters up in a table instead of searching base64_chars
//  - encoding and decoding: size the output once and write through a pointer (no library call per character),
//    whole groups of 3 bytes / 4 characters, validation through the decoding table

// The encoding and decoding loops were rewritten for Elpida (no code copied), using techniques of these
// implementations, credited here:
//  - OpenSSL (crypto/evp/encode.c, The OpenSSL Project Authors, Apache-2.0): decoding table with 0xFF for invalid
//    characters, and one check of a group of 4 decoded values: (a | b | c | d) & 0x80.
//  - Go (encoding/base64, The Go Authors, BSD-style): 256-entry decodeMap with 0xFF for invalid characters, an OR
//    check per group, and encoding into an output sized in advance ((n + 2) / 3 * 4).
//  - modp_b64 (stringencoders, Nick Galbreath, MIT; also in Chromium): per-group table lookups with one
//    bad-character check per group, and output written through a pointer.
// This keeps the timed loops our own code, the same machine code everywhere, with no data-dependent branch per
// character. See docs/development/base64-decode-spread.md.

#include "Base64.hpp"

namespace Elpida::Benchmarks::Web
{
	static constexpr std::string_view base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
											"abcdefghijklmnopqrstuvwxyz"
											"0123456789+/";

	std::string Base64::Encode(const unsigned char* data, const std::size_t size)
	{
		std::string ret;
		ret.resize((size + 2) / 3 * 4);	// exact output size

		auto out = ret.data();
		constexpr auto chars = base64_chars.data();

		for (const auto end = data + (size - size % 3);
			data != end;
			data += 3, out += 4)
		{
			const unsigned char a = data[0], b = data[1], c = data[2];
			out[0] = chars[a >> 2];
			out[1] = chars[((a & 0x03) << 4) | (b >> 4)];
			out[2] = chars[((b & 0x0f) << 2) | (c >> 6)];
			out[3] = chars[c & 0x3f];
		}

		switch (size % 3)
		{
		case 1:
			out[0] = chars[data[0] >> 2];
			out[1] = chars[(data[0] & 0x03) << 4];
			out[2] = '=';
			out[3] = '=';
			break;
		case 2:
			out[0] = chars[data[0] >> 2];
			out[1] = chars[((data[0] & 0x03) << 4) | (data[1] >> 4)];
			out[2] = chars[(data[1] & 0x0f) << 2];
			out[3] = '=';
			break;
		default:
			break;
		}

		return ret;
	}

	// Character -> 6-bit value, or Invalid (bit 7 set) for '=' and every character outside the alphabet.
	// A table, not a search of base64_chars: a search loop exits after a data-dependent number of steps, which some
	// CPUs predict well, some badly and some depending on the address layout of the process (11% run-to-run on an
	// i9-10900K), and a compiler may or may not turn it into range checks.
	struct Base64DecodeTable
	{
		static constexpr unsigned char Invalid = 0xFF;

		unsigned char values[256];

		constexpr Base64DecodeTable() : values()
		{
			for (auto& value : values)
			{
				value = Invalid;
			}

			for (std::size_t i = 0; i < 64; ++i)
			{
				values[static_cast<unsigned char>(base64_chars[i])] = static_cast<unsigned char>(i);
			}
		}
	};

	static constexpr Base64DecodeTable base64DecodeTable{};

	// Decodes up to the end, the first '=' or the first character outside the alphabet, whichever comes first.
	std::string Base64::Decode(const unsigned char* data, std::size_t size)
	{
		auto& values = base64DecodeTable.values;
		const auto end = data + size;

		std::string ret;
		ret.resize(size / 4 * 3 + 2);	// upper bound, cut to the real size at the end
		auto out = reinterpret_cast<unsigned char*>(ret.data());

		// Whole groups of 4 characters: one check per group, not per character
		while (end - data >= 4)
		{
			const unsigned char a = values[data[0]], b = values[data[1]], c = values[data[2]], d = values[data[3]];
			if ((a | b | c | d) & 0x80) break;	// '=' or invalid character in this group

			out[0] = static_cast<unsigned char>((a << 2) | (b >> 4));
			out[1] = static_cast<unsigned char>((b << 4) | (c >> 2));
			out[2] = static_cast<unsigned char>((c << 6) | d);
			data += 4;
			out += 3;
		}

		// The rest: at most 3 valid characters, before the end or before the character that stopped the loop
		unsigned char v[3] = {};
		std::size_t n = 0;
		while (n < 3 && data != end && !(values[*data] & 0x80))
		{
			v[n++] = values[*data++];
		}

		if (n >= 2) *out++ = static_cast<unsigned char>((v[0] << 2) | (v[1] >> 4));
		if (n == 3) *out++ = static_cast<unsigned char>((v[1] << 4) | (v[2] >> 2));

		ret.resize(reinterpret_cast<char*>(out) - ret.data());
		return ret;
	}
} // Elpida