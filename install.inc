<?php

//We set a version, so we can control if there are any upgrades etc.
$_INSTALL['VERSION'] = 1.09;


//We also create a menu item to be added to the main menu placed in database
$_INSTALL['MAIN_MENU_ITEM'] = array (
    'title' => 'Newsletter', // Title of the menu item
    'url' => '/newsletter/subscribe', // url of the item
    'module_name' => 'newsletter', // beloning to which module
    'parent' => 0, // does it have a parent item
    'weight' => 25, // weight
);

if (config::getModuleIni('newsletter_menu_admin_only')) {
    $_INSTALL['MAIN_MENU_ITEM']['auth'] = 'admin';
}

$_INSTALL['PUBLIC_CLONE_URL'] = 'git://github.com/diversen/newsletter.git';
$_INSTALL['PRIVATE_CLONE_URL'] = 'git@github.com:diversen/newsletter.git';