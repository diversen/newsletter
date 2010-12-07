<?php
/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

//echo uri::$fragments[2];

template::setTitle(lang::translate('newsletter_verify_unsubscribe_title'));
$newsletter = new newsletter();
$newsletter->verifyUnsubscriber();

