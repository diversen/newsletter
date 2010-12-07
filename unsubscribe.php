<?php

template::setTitle(lang::translate('unsubscribe_to_newsletter'));
if (isset($_POST['submit'])){
    newsletter::validate('unsubscribe');
    if (empty(newsletter::$errors)){
        $res = newsletter::deleteSubscriber();
        if ($res){
            //session::setActionMessage(lang::translate('subscribed_message'));
            echo lang::translate('unsubscribed_message');
            return;
        }
    } else {
        view_form_errors(newsletter::$errors);
    }
}

newsletter::unsubscribeform();