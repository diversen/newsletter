<?php

template::setTitle(lang::translate('newsletter_unsubscribe_to_newsletter'));
if (isset($_POST['submit'])){
    newsletter::validate('unsubscribe');
    if (empty(newsletter::$errors)){
        $res = newsletter::deleteSubscriber();
        if ($res){
            echo lang::translate('newsletter_unsubscribed_message');
            return;
        }
    } else {
        view_form_errors(newsletter::$errors);
    }
}

newsletter::unsubscribeform();