<?php
	/*  Scripted Roulette - version 0.2.1
	 *  Copyright (C) 2015-2017, http://scripted-roulette.sourceforge.net
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
	 */


	//-- Gets the content of a parameter
	function GetGet($pName, $pDefault)
	{
		if (isset($_GET[$pName]))
			return $_GET[$pName];
		else
			return $pDefault;
	}

	//-- Gets the content of a parameter on server side
	function GetSession($pName, $pDefault)
	{
		if (isset($_SESSION[$pName]))
			return $_SESSION[$pName];
		else
			return $pDefault;
	}

	//-- Gets the quota
	function GetQuota()
	{
		return GetSession('quota', 1000000);
	}

	//-- Sets the quota
	function SetQuota($pValue)
	{
		if ($pValue < 0)
			$pValue = 0;
		$_SESSION['quota'] = $pValue;
	}

	//-- Session management
	session_start();
	$_SESSION['quota'] = GetQuota();
?>