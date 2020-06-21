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

namespace Elpida
{

	DataUploader::DataUploader(Mediator& mediator, const ResultFormatter& resultFormatter)
		: _networkAccessManager(), _mediator(mediator), _resultFormatter(resultFormatter)
	{
		QObject::connect(&_networkAccessManager, &QNetworkAccessManager::finished, this, &DataUploader::onFinished);
		QObject::connect(this, &DataUploader::uploadRequest, this, &DataUploader::onUploadRequested);
	}

	void DataUploader::handle(UploadResultCommand& command)
	{
		emit uploadRequest(&command.getBenchmarkResult());
	}

	void DataUploader::onFinished(QNetworkReply* reply)
	{
		if (reply->error()) {
			qDebug() << reply->errorString();
			return;
		}

		QString answer = reply->readAll();

		qDebug() << answer;
	}

	void DataUploader::onUploadRequested(const BenchmarkResult* result)
	{
		QNetworkRequest request;
		request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
		request.setUrl(QUrl("https://localhost:5001/api/result"));
		auto serialized = _resultFormatter.serialize(*result);
		auto reply = _networkAccessManager.post(request, QByteArray(serialized.c_str(),serialized.size()));
		connect(reply, &QIODevice::readyRead, [reply](){
			if (reply->error()) {
				qDebug() << reply->errorString();
				return;
			}

			QString answer = reply->readAll();

			qDebug() << answer;
		});

		connect(reply, &QIODevice::readyRead,[reply](){
			if (reply->error()) {
				qDebug() << reply->errorString();
				return;
			}

			QString answer = reply->readAll();

			qDebug() << answer;
		});
		connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),[reply](){
			if (reply->error()) {
				qDebug() << reply->errorString();
				return;
			}

			QString answer = reply->readAll();

			qDebug() << answer;
		});
		connect(reply, &QNetworkReply::sslErrors,[reply](){
			reply->ignoreSslErrors();
			return;
			if (reply->error()) {
				qDebug() << reply->errorString();
				return;
			}

			QString answer = reply->readAll();

			qDebug() << answer;
		});
	}

}