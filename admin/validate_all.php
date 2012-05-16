<?php

include_module ("newsletter");

if (!session::checkAccessControl('newsletter_allow_send_news')){
    return;
}

if (!isset($_POST['submit']) && !isset($_POST['delete'])){
    newsletter::displayValidateAllEmails();
}
if (isset($_POST['submit'])){
    newsletter::validateAllEmails();
}

if (isset($_POST['delete'])){
    newsletter::deleteBrokenEmails();
    session::setActionMessage(lang::translate('newsletter_broken_emails_deleted'));
}
