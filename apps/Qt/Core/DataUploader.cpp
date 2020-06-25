/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 20/6/20.
//

#include "DataUploader.hpp"

#include "Commands/UploadResultCommand.hpp"
#include "Commands/HttpResponseEvent.hpp"
#include "Core/Abstractions/Mediator.hpp"
#include "Core/Abstractions/ResultFormatter.hpp"

#include <QtNetwork/QNetworkReply>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>

namespace Elpida
{

	DataUploader::DataUploader(Mediator& mediator, const ResultFormatter& resultFormatter, Logger& logger)
		: _networkAccessManager(), _mediator(mediator), _resultFormatter(resultFormatter), _logger(logger)
	{
		QObject::connect(&_networkAccessManager, &QNetworkAccessManager::finished, this, &DataUploader::onFinished);
		QObject::connect(&_networkAccessManager, &QNetworkAccessManager::sslErrors, this, &DataUploader::onSslErrors);
		QObject::connect(this, &DataUploader::uploadRequest, this, &DataUploader::onUploadRequested);
	}

	void DataUploader::handle(UploadResultCommand& command)
	{
		emit uploadRequest(&command.getBenchmarkResult());
	}

	void DataUploader::onFinished(QNetworkReply* reply)
	{
		auto responseCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
		if (reply->error())
		{
			auto err = reply->errorString();
			_mediator.execute(HttpResponseEvent(err.toStdString(), responseCode.value<int>()));
			return;
		}
		QString answer = reply->readAll();
		_mediator.execute(HttpResponseEvent(answer.toStdString(), responseCode.value<int>()));
	}

	void DataUploader::onUploadRequested(const BenchmarkResult* result)
	{
		QNetworkRequest request;
		request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
		request.setUrl(QUrl(apiUrl));
		auto serialized = _resultFormatter.serialize(*result);
		_networkAccessManager.post(request, QByteArray(serialized.c_str(), serialized.size()));
	}

	void DataUploader::onSslErrors(QNetworkReply* reply, const QList<QSslError>& errors)
	{
		for (const auto& err: errors)
		{
			_logger.log(LogType::Error, Vu::Cs(
				"SSL Errors occurred when handling the request: '",
				reply != nullptr ? reply->request().url().toString().toStdString() : "[Unknown]",
				"'. ",
				err.errorString().toStdString()));
		}
	}

}