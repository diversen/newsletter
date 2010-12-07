<?php



template::setTitle(lang::translate('subscribe_message'));
template::setTitle(lang::translate('subscribe_to_newsletter'));
if (isset($_POST['submit'])){
    newsletter::validate();
    if (empty(newsletter::$errors)){
        $res = newsletter::addSubscriber();
        if ($res){
            //session::setActionMessage(lang::translate('subscribed_message'));
            echo lang::translate('subscribed_message');
            return;
        }
    } else {
        view_form_errors(newsletter::$errors);
    }
}

newsletter::subscribeform();



