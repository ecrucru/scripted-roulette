<?php
	//Scripted Roulette - GPLv2

	function GetServer($pName, $pDefault)
	{
		if (isset($_SERVER[$pName]))
			return $_SERVER[$pName];
		else
			return $pDefault;
	}

	header('Content-Type: text/html');
	if ((substr_count(GetServer('HTTP_ACCEPT_ENCODING', ''), 'gzip') > 0)
	 && (substr_count(GetServer('HTTP_USER_AGENT', ''), 'MSIE') == 0))
	{
		header('Content-Encoding: gzip');
		echo file_get_contents('page_compressed.gz');
	}
	else
		echo file_get_contents('page_uncompressed.html');
?>