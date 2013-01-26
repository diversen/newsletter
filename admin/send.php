<?php
/**
 * controller for content/admin/add_article
 * it just includes the controller content/article/add
 *
 * @package    content
 */

moduleloader::includeModule ("newsletter");

if (!session::checkAccessControl('newsletter_allow_send_news')){
    return;
}

$newsletter = new newsletter();
$newsletter->validateSend();

view_confirm(lang::translate('newsletter_admin_newsletter_note'));

if ( isset($_POST['submit']) && @empty($_POST['test_email'])){
    if (empty(newsletter::$errors)){
        $newsletter->send();
        print lang::translate('newsletter_has_been_sent');
        return;
    } else {
        view_form_errors(newsletter::$errors);
    }
}

if ( isset($_POST['submit']) && @!empty($_POST['test_email'])){
    if (empty(newsletter::$errors)){
        $newsletter->send();
        print lang::translate('newsletter_test_has_been_sent');
        //return;
    } else {
        view_form_errors(newsletter::$errors);
    }
}

include _COS_PATH . "/modules/newsletter/views/form_send.inc";
