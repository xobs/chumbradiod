/*
 * chumby::HTTPContentManager
 * Copyright (C) Chumby Industries, 2007
 *
 * based on Artemis Copyright (C) 2007 Andreas Buechele
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _CHUMBY_HTTP_CONTENT_MANAGER_H
#define _CHUMBY_HTTP_CONTENT_MANAGER_H

#include <list>
#include <chumby_httpd/chumby_http_request.h>
#include <chumby_httpd/chumby_http_content_handler.h>
#include <chumby_httpd/chumby_http_response.h>

namespace chumby {
	class HTTPContentManager {
	public:
		HTTPContentManager();
		~HTTPContentManager();

		void addContentHandler(chumby::HTTPContentHandler * contentHandler);
		chumby::HTTPResponse * handleRequest(const chumby::HTTPRequest & request);
	private:
		std::list<chumby::HTTPContentHandler *> * _contentHandlers;
	};
}

#endif
