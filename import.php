<?php

//include_once "lib/shell.php";

$db = new db();
$db->connect();

$file_content = file_get_contents(_COS_PATH . "/modules/newsletter/emails.txt");

$ary = explode ("\n\n", $file_content);
//print_r($ary);

foreach ($ary as $key => $val){

    $ary2 = explode ("\n", $val);

    $values = array ('subscribed' => 1, 'name' => trim($ary2[1]), 'email' => trim($ary2[0]));

    if (empty($values['email'])) continue;

    $db->insert('newsletter', $values);
}