<?php

include_module ("newsletter");

if (!session::checkAccessControl('allow_send_news')){
    return;
}

if (!isset($_POST['submit']) && !isset($_POST['delete'])){
    newsletter::validateAlllEmailsForm();
}
if (isset($_POST['submit'])){
    newsletter::validateAllEmails();
}

if (isset($_POST['delete'])){
    newsletter::deleteBrokenEmails();
    session::setActionMessage(lang::translate('broken_emails_deleted'));
}
