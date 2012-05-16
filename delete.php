<?php

if (!session::checkAccessControl('newsletter_allow_send_news')){
    return;
}

$news = new newsletter();
$id = uri::$fragments[2];
$row = $news->getNewsletter($id);

if (!isset($id) || !isset($row)){
    header ("Location: /newsletter/archive");
}

html::$autoLoadTrigger = 'submit';
//html::init($vars);
html::formStart('newsletter_delete_form');
html::legend(lang::translate('newsletter_delete_label') . MENU_SUB_SEPARATOR_SEC . $row['title']);
html::submit('submit', lang::translate('newsletter_delete_button'));
html::formEnd();

echo html::$formStr;

if (isset($_POST['submit'])) {
    $news->deleteNewsletter($id);
    session::setActionMessage(lang::translate('newsletter_delete_action_message'));
    header("Location: /newsletter/archive");
    die();
}