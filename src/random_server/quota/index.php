<?php
	/*  Scripted Roulette - version 0.1
	 *  Copyright (C) 2015-2016, http://scripted-roulette.sourceforge.net
	 *
	 *  This program is free software; you can redistribute it and/or modify
	 *  it under the terms of the GNU General Public License as published by
	 *  the Free Software Foundation; either version 2 of the License, or
	 *  (at your option) any later version.
	 *
	 *  This program is distributed in the hope that it will be useful,
	 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
	 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	 *  GNU General Public License for more details.
	 *
	 *  You should have received a copy of the GNU General Public License along
	 *  with this program; if not, write to the Free Software Foundation, Inc.,
	 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
	 *
	 *  ---------------------------------------------------------------------------
	 *  This script is a fake emulation of this page :
	 *      https://www.random.org/quota/?format=plain
	 *
	 *  It is used to test the development of Scripted Roulette without overloading
	 *  this online service.
	 *
	 *  To simplify, the check here is based on the cookies, not IP addresses else
	 *  it will require to manage a database or a file.
	 */


	include '../lib.php';

	//-- Checks
	if (GetGet('format', 'plain') <> 'plain')
	{
		header("HTTP/1.0 503 Service Unavailable");
		die(); //Unsupported parameters
	}

	//-- Displays the quota
	header("Content-Type: text/plain");
	echo GetQuota();
?>