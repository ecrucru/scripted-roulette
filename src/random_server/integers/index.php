<?php
	/*  Scripted Roulette - version 0.2
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
	 *      https://www.random.org/integers/?num=10&min=1&max=6&col=1&base=10&format=plain&rnd=new
	 *
	 *  It is used to test the development of Scripted Roulette without overloading
	 *  this online service.
	 */


	include '../lib.php';

	//-- Gets the parameters
	$num = (int) GetGet('num', 1);
	$min = (int) GetGet('min', 0);
	$max = (int) GetGet('max', 0);
	$col = (int) GetGet('col', 1);
	$base = (int) GetGet('base', 10);
	$format = GetGet('format', 'plain');
	$rnd = GetGet('rnd', 'new');

	//-- Checks
	if (	($num < 1) || ($num > 1000) ||
			($min >= $max) ||
			($min < -1000000000) || ($min > 1000000000) ||
			($max < -1000000000) || ($max > 1000000000) ||
			($col <> 1) ||
			($base <> 10) ||
			($format <> 'plain') ||
			($rnd <> 'new')
		)
	{
		header("HTTP/1.0 503 Service Unavailable");
		die(); //Unsupported parameters
	}

	//-- Writes the random numbers
	header("Content-Type: text/plain");
	$quota = GetQuota();
	for ($i=0 ; $i<$num ; $i++)
	{
		//- No quota, no work
		if ($quota == 0)
			break;

		//- Gets a random value : the quality of the numbers doesn't matter as the objective is purely technical
		$value = rand($min, $max);

		//- Consumes the quota
		$bits = strlen((string)decbin($value));
		if ($bits > $quota)
			$quota = 0;
		else
			$quota -= $bits;

		//- Shows the value
		if ($i > 0)
			echo "\n";
		echo $value;
	}
	SetQuota($quota);
?>