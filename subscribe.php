<?php

template::setTitle(lang::translate('newsletter_subscribe_message'));
template::setTitle(lang::translate('newsletter_subscribe_to_newsletter'));
if (isset($_POST['submit'])){
    newsletter::validate();
    if (empty(newsletter::$errors)){
        $res = newsletter::addSubscriber();
        if ($res){
            echo lang::translate('newsletter_subscribed_message');
            return;
        }
    } else {
        view_form_errors(newsletter::$errors);
    }
}

newsletter::subscribeform();

