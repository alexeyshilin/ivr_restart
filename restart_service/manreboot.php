<?php

/*
###############################################################################
$REQUEST_METHOD = getenv('REQUEST_METHOD');
switch($REQUEST_METHOD){
	case 'GET':
		$HTTP_VARS = &$HTTP_GET_VARS;
		break;
	case 'POST':
		$HTTP_VARS = &$HTTP_POST_VARS;
        break;
}
$VARS = &$HTTP_VARS;
###############################################################################
while(list($key,$val) = @each($VARS)){
	${$key} = $val;
} 
###############################################################################
*/

###############################################################################
while(list($key,$val) = @each($HTTP_GET_VARS)){
	$HTTP_VARS[$key] = $val;
}
while(list($key,$val) = @each($HTTP_POST_VARS)){
	$HTTP_VARS[$key] = $val;
}
$VARS = &$HTTP_VARS;
###############################################################################
while(list($key,$val) = @each($VARS)){
	${$key} = $val;
} 
###############################################################################

$ROOT = getenv('DOCUMENT_ROOT');
$QUERY = getenv('QUERY_STRING');
$RMETHOD = getenv('REQUEST_METHOD');


$ROOT = "c:\Inetpub\wwwroot\someadmin";

if($QUERY=='phpinfo'){
	phpinfo();
}

define(INCLUDE_PATH,$ROOT.'/include');
require_once($ROOT.'/config/db.php');
require_once(INCLUDE_PATH.'/db.inc');
require_once(INCLUDE_PATH.'/utils.php');
$script = '/'.getenv('SCRIPT_NAME');


//print("[$ACTION]");

if($ACTION=="manreboot"){
	$filename = "c:\\pipec\\reboot.xml";
	$data = "pipec manreboot";
	saveToFile($filename,$data,"w");
	
	print("[ну вроде ок]");
}else{
	print('<a href="?ACTION=manreboot">может не надо?</a>');
}

?>