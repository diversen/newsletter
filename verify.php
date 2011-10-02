<?php


template::setTitle(lang::translate('newsletter_verify_title'));
$newsletter = new newsletter();
$newsletter->verifySubscriber();
