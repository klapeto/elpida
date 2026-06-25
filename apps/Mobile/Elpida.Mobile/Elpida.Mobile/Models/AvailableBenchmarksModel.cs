using System.Collections.Generic;
namespace Elpida.Mobile.Models
{
	public class AvailableBenchmarksModel
	{
		public List<BenchmarkInfoModel> Benchmarks { get; set; } = new()
		{
			// Compilation
			new BenchmarkInfoModel
			{
				Name = "C++ Compilation",
				Description = "Compiles C++ code with Clang",
				ResultUnit = "Files",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "C++ source generation",
						Description = "Generates C++ source code",
						ResultUnit = "Chars",
						ResultDescription = "The byte generation throughput",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Clang compilation",
						Description = "Compiles a file with clang",
						ResultUnit = "Files",
						ResultDescription = "The compilation file throughput",
						ResultType = ResultType.Throughput,
						IsMeasured = true,
					},
				}
			},
			// Compression
			new BenchmarkInfoModel
			{
				Name = "Zlib compression",
				Description = "Compresses a data with zlib",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Generate Lorem Ipsum text",
						Description = "Generates random Lorem Ipsum text",
						ResultUnit = "chars",
						ResultDescription = "The rate of the generation",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Zlib compression",
						Description = "Compresses data with Zlib",
						ResultUnit = "B",
						ResultDescription = "The rate of compression",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Zlib decompression",
				Description = "Decompresses a data with zlib",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Generate Lorem Ipsum text",
						Description = "Generates random Lorem Ipsum text",
						ResultUnit = "chars",
						ResultDescription = "The rate of the generation",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Zlib compression",
						Description = "Compresses data with Zlib",
						ResultUnit = "B",
						ResultDescription = "The rate of compression",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Zlib Decompression",
						Description = "Decompresses data with Zlib",
						ResultUnit = "B",
						ResultDescription = "The rate of decompression",
						ResultType = ResultType.Throughput,
					},
				}
			},
			// Encryption
			new BenchmarkInfoModel
			{
				Name = "AES Decryption",
				Description = "Decrypts data with a AES",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Generate Lorem Ipsum text",
						Description = "Generates random Lorem Ipsum text",
						ResultUnit = "chars",
						ResultDescription = "The rate of the generation",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "AES encryption",
						Description = "Encrypts data using AES",
						ResultUnit = "B",
						ResultDescription = "The rate of encryption",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "AES decryption",
						Description = "Decrypts data using AES",
						ResultUnit = "B",
						ResultDescription = "The rate of decryption",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "AES Encryption",
				Description = "Encrypts data with a AES.",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Generate Lorem Ipsum text",
						Description = "Generates random Lorem Ipsum text",
						ResultUnit = "chars",
						ResultDescription = "The rate of the generation",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "AES encryption",
						Description = "Encrypts data using AES",
						ResultUnit = "B",
						ResultDescription = "The rate of encryption",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "RSA Decryption",
				Description = "Decrypts data with a private key.",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Generate Lorem Ipsum text",
						Description = "Generates random Lorem Ipsum text",
						ResultUnit = "chars",
						ResultDescription = "The rate of the generation",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "RSA encryption",
						Description = "Encrypts data using public/private keys",
						ResultUnit = "B",
						ResultDescription = "The rate of encryption",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "RSA decryption",
						Description = "Decrypts data using public/private keys",
						ResultUnit = "B",
						ResultDescription = "The rate of decryption",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "RSA Encryption",
				Description = "Encrypts data with a public key.",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Generate Lorem Ipsum text",
						Description = "Generates random Lorem Ipsum text",
						ResultUnit = "chars",
						ResultDescription = "The rate of the generation",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "RSA encryption",
						Description = "Encrypts data using public/private keys",
						ResultUnit = "B",
						ResultDescription = "The rate of encryption",
						ResultType = ResultType.Throughput,
					},
				}
			},
			// Image
			new BenchmarkInfoModel
			{
				Name = "PngEncoding",
				Description = "Encodes an image file to PNG.",
				ResultUnit = "Pixels",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "File read",
						Description = "Reads a file from the filesystem to memory.",
						ResultUnit = "B",
						ResultDescription = "The amount of bytes read per second",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "PNG decoding",
						Description = "Decodes a stream of data from PNG decoded data to RGBA data.",
						ResultUnit = "B",
						ResultDescription = "The input amount of bytes processed per second",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Png encoding",
						Description = "Encodes data to png format.",
						ResultUnit = "Pixels",
						ResultDescription = "The amount of pixels encoded.",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "File write",
						Description = "Writes a file from the memory to filesystem.",
						ResultUnit = "B",
						ResultDescription = "The amount of bytes written per second",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "3D Ray Tracing",
				Description = "Ray traces a 3D scene to rasterized image",
				ResultUnit = "Rays",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "3D Ray Tracing",
						Description = "Ray traces a 3D scene to rasterized image",
						ResultUnit = "Rays",
						ResultDescription = "The ray tracing rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Svg Parsing",
				Description = "Parses an xml parsed document to SVG Dom tree.",
				ResultUnit = "Elements",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "File read",
						Description = "Reads a file from the filesystem to memory.",
						ResultUnit = "B",
						ResultDescription = "The amount of bytes read per second",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "XML Parsing",
						Description = "Parses an XML document and measures the parsing throughput",
						ResultUnit = "chars",
						ResultDescription = "How many characters are processes in the time",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "SVG Parsing",
						Description = "Parses a SVG document and measures the parsing throughput",
						ResultUnit = "Elements",
						ResultDescription = "How many elements are processed in the time",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Svg Rasterization 2",
				Description = "Rasterizes a calculated Svg document. Uses multiple buffers instead of a single.",
				ResultUnit = "pixels",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "File read",
						Description = "Reads a file from the filesystem to memory.",
						ResultUnit = "B",
						ResultDescription = "The amount of bytes read per second",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "XML Parsing",
						Description = "Parses an XML document and measures the parsing throughput",
						ResultUnit = "chars",
						ResultDescription = "How many characters are processes in the time",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "SVG Parsing",
						Description = "Parses a SVG document and measures the parsing throughput",
						ResultUnit = "Elements",
						ResultDescription = "How many elements are processed in the time",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "SVG Calculation",
						Description = "Calculates lengths and transformations and scales the values to the desired scale",
						ResultUnit = "Elements",
						ResultDescription = "How many elements are processes in the time",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Svg Rasterization 2",
						Description = "Rasterizes a calculated Svg document. Uses multiple buffers instead of a single.",
						ResultUnit = "pixels",
						ResultDescription = "How many pixels per second are calculated on average",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Convert to UInt8",
						Description = "Converts RGBA float pixels to uint8 channels (0 - 255)",
						ResultUnit = "Pixels",
						ResultDescription = "The amount of pixels processed per second.",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Png encoding",
						Description = "Encodes data to png format.",
						ResultUnit = "Pixels",
						ResultDescription = "The amount of pixels encoded.",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "File write",
						Description = "Writes a file from the memory to filesystem.",
						ResultUnit = "B",
						ResultDescription = "The amount of bytes written per second",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Svg Rasterization",
				Description = "Rasterizes a calculated Svg document.",
				ResultUnit = "pixels",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Svg Document Generation",
						Description = "Generates a document with n amount of elements",
						ResultUnit = "B",
						ResultDescription = "How many bytes are generated",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "XML Parsing",
						Description = "Parses an XML document and measures the parsing throughput",
						ResultUnit = "chars",
						ResultDescription = "How many characters are processes in the time",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "SVG Parsing",
						Description = "Parses a SVG document and measures the parsing throughput",
						ResultUnit = "Elements",
						ResultDescription = "How many elements are processed in the time",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "SVG Calculation",
						Description = "Calculates lengths and transformations and scales the values to the desired scale",
						ResultUnit = "Elements",
						ResultDescription = "How many elements are processes in the time",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Svg Rasterization",
						Description = "Rasterizes a calculated Svg document",
						ResultUnit = "pixels",
						ResultDescription = "How many pixels per second are calculated on average",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Svg Rasterization from file",
				Description = "Rasterizes a calculated Svg document from the disk.",
				ResultUnit = "pixels",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "File read",
						Description = "Reads a file from the filesystem to memory.",
						ResultUnit = "B",
						ResultDescription = "The amount of bytes read per second",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "XML Parsing",
						Description = "Parses an XML document and measures the parsing throughput",
						ResultUnit = "chars",
						ResultDescription = "How many characters are processes in the time",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "SVG Parsing",
						Description = "Parses a SVG document and measures the parsing throughput",
						ResultUnit = "Elements",
						ResultDescription = "How many elements are processed in the time",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "SVG Calculation",
						Description = "Calculates lengths and transformations and scales the values to the desired scale",
						ResultUnit = "Elements",
						ResultDescription = "How many elements are processes in the time",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Svg Rasterization",
						Description = "Rasterizes a calculated Svg document",
						ResultUnit = "pixels",
						ResultDescription = "How many pixels per second are calculated on average",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Convert to UInt8",
						Description = "Converts RGBA float pixels to uint8 channels (0 - 255)",
						ResultUnit = "Pixels",
						ResultDescription = "The amount of pixels processed per second.",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Png encoding",
						Description = "Encodes data to png format.",
						ResultUnit = "Pixels",
						ResultDescription = "The amount of pixels encoded.",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "File write",
						Description = "Writes a file from the memory to filesystem.",
						ResultUnit = "B",
						ResultDescription = "The amount of bytes written per second",
						ResultType = ResultType.Throughput,
					},
				}
			},
			// Math
			new BenchmarkInfoModel
			{
				Name = "FFT calculation in place",
				Description = "Calculates the DFT of values in place",
				ResultUnit = "numbers",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Calculate FFT in place",
						Description = "Calculates the DTF of a series of complex numbers",
						ResultUnit = "numbers",
						ResultDescription = "The rate of number processing",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Dynamic matrix multiplication",
				Description = "Calculates the product between 2 matrices that their sizes are not known in advance",
				ResultUnit = "numbers",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Dynamic matrix multiplication",
						Description = "Multiplies 2 matrices that their dimensions are not known in advanced",
						ResultUnit = "numbers",
						ResultDescription = "The amount of numbers processed per second",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Matrix inverse (4x4)",
				Description = "Calculates the inverse of a 4x4 matrix",
				ResultUnit = "Numbers",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Matrix inverse (4x4)",
						Description = "Calculates the inverse of a 4x4 matrix",
						ResultUnit = "Numbers",
						ResultDescription = "The number process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Matrix multiplication (32x32)",
				Description = "Calculates the product between 2 32x23 matrices",
				ResultUnit = "Numbers",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Matrix multiplication (32x32)",
						Description = "Calculates the product between 2 32x23 matrices",
						ResultUnit = "Numbers",
						ResultDescription = "The amount of numbers processed per second",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "NBody Simulation",
				Description = "Performs NBody simulation",
				ResultUnit = "Iterations",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "NBody",
						Description = "Performs NBody simulation",
						ResultUnit = "Iterations",
						ResultDescription = "The iteration process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			// Memory
			new BenchmarkInfoModel
			{
				Name = "Memory latency",
				Description = "Calculates the average memory access latency by randomly reading memory regions.",
				ResultUnit = "s",
				ResultType = ResultType.Time,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Allocate memory",
						Description = "Allocates a chunk of memory to be used by other tasks.",
						ResultUnit = "B",
						ResultDescription = "The amount of bytes allocated per second",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Memory latency",
						Description = "Calculates the average access latency of memory.",
						ResultUnit = "s",
						ResultDescription = "The average access time.",
						ResultType = ResultType.Time,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Memory read bandwidth",
				Description = "Calculates the peak memory read bandwidth by continuously reading a memory stream.",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Allocate memory",
						Description = "Allocates a chunk of memory to be used by other tasks.",
						ResultUnit = "B",
						ResultDescription = "The amount of bytes allocated per second",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Memory read bandwidth",
						Description = "Continuously reads a stream a memory to determine th maximum memory read bandwidth.",
						ResultUnit = "B",
						ResultDescription = "The memory bandwidth throughput",
						ResultType = ResultType.Throughput,
					},
				}
			},
			// StdLib
			new BenchmarkInfoModel
			{
				Name = "ceil()",
				Description = "Measures the ceil() throughput",
				ResultUnit = "number",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "ceil()",
						Description = "Measures the ceil() performance",
						ResultUnit = "number",
						ResultDescription = "The number process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "floor()",
				Description = "Measures the floor() throughput",
				ResultUnit = "numbers",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "floor()",
						Description = "Measures the floor() performance",
						ResultUnit = "numbers",
						ResultDescription = "The number process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "malloc()",
				Description = "Measures the malloc() throughput",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Malloc rate",
						Description = "Measures the allocation rate",
						ResultUnit = "B",
						ResultDescription = "The allocation rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "memchr()",
				Description = "Measures the memchr() throughput",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Allocate memory",
						Description = "Allocates a chunk of memory to be used by other tasks.",
						ResultUnit = "B",
						ResultDescription = "The amount of bytes allocated per second",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Memchr",
						Description = "Memchr Description",
						ResultUnit = "B",
						ResultDescription = "The process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "memset()",
				Description = "Measures the memset() throughput",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Allocate memory",
						Description = "Allocates a chunk of memory to be used by other tasks.",
						ResultUnit = "B",
						ResultDescription = "The amount of bytes allocated per second",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Memset",
						Description = "Memset Description",
						ResultUnit = "B",
						ResultDescription = "The process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Regex",
				Description = "Searches text with regex",
				ResultUnit = "Chars",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Regex (StdLib)",
						Description = "Searches text with regex",
						ResultUnit = "Chars",
						ResultDescription = "The character process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "sqrt()",
				Description = "Measures the sqrt() throughput",
				ResultUnit = "numbers",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "sqrt()",
						Description = "Measures the sqrt() performance",
						ResultUnit = "numbers",
						ResultDescription = "The number process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "strtod()",
				Description = "Measures the strtod() throughput",
				ResultUnit = "chars",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Strtod",
						Description = "Measures the strtod() performance",
						ResultUnit = "chars",
						ResultDescription = "The character process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "strtoul()",
				Description = "Measures the strtoul() throughput",
				ResultUnit = "chars",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Strtoul",
						Description = "Measures the strtoul() performance",
						ResultUnit = "chars",
						ResultDescription = "The character process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			// Web
			new BenchmarkInfoModel
			{
				Name = "Base64 Decode",
				Description = "Decodes base64 encoded data",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Generate Lorem Ipsum text",
						Description = "Generates random Lorem Ipsum text",
						ResultUnit = "chars",
						ResultDescription = "The rate of the generation",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Base64 Encode",
						Description = "Encodes data to base64",
						ResultUnit = "B",
						ResultDescription = "The data process rate",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Base64 Decode",
						Description = "Decodes base64 encoded data",
						ResultUnit = "B",
						ResultDescription = "The data process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Base64 Encode",
				Description = "Encodes data to base64",
				ResultUnit = "B",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Generate Lorem Ipsum text",
						Description = "Generates random Lorem Ipsum text",
						ResultUnit = "chars",
						ResultDescription = "The rate of the generation",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Base64 Encode",
						Description = "Encodes data to base64",
						ResultUnit = "B",
						ResultDescription = "The data process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Json Parsing",
				Description = "Parses a Json document to determine the parsing speed.",
				ResultUnit = "chars",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Json generation",
						Description = "Generates a json file in memory with the provided size",
						ResultUnit = "B",
						ResultDescription = "The rate of generation",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "JSON Parsing",
						Description = "Parses an JSON document and measures the parsing throughput",
						ResultUnit = "chars",
						ResultDescription = "How many characters are processes in the time",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "Regex (Boost)",
				Description = "Searches text with regex",
				ResultUnit = "Chars",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Generate Lorem Ipsum text",
						Description = "Generates random Lorem Ipsum text",
						ResultUnit = "chars",
						ResultDescription = "The rate of the generation",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "Regex (Boost)",
						Description = "Searches text with regex",
						ResultUnit = "Chars",
						ResultDescription = "The character process rate",
						ResultType = ResultType.Throughput,
					},
				}
			},
			new BenchmarkInfoModel
			{
				Name = "XML Parsing",
				Description = "Parses an XML document to determine the parsing speed.",
				ResultUnit = "chars",
				ResultType = ResultType.Throughput,
				TaskInfos = new()
				{
					new TaskInfoModel
					{
						Name = "Xml generation",
						Description = "Generates a xml file in memory with the provided size",
						ResultUnit = "B",
						ResultDescription = "The rate of generation",
						ResultType = ResultType.Throughput,
					},
					new TaskInfoModel
					{
						Name = "XML Parsing",
						Description = "Parses an XML document and measures the parsing throughput",
						ResultUnit = "chars",
						ResultDescription = "How many characters are processes in the time",
						ResultType = ResultType.Throughput,
					},
				}
			},
		};
	}
}