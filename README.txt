README

<h1>Module tutorial</h1>

This is a newsletter module. At the same time it is a tutorial in making a
small module, which is not a blog .)

When you create a base module you can start with the command:

./coscli.sh skeleton --create-module newsletter

This creates all the module files which you can se in the newletter dir.

First file to edit is the install.inc file, because the system is in such a
way that you can not read any files from client side unless they have been
registered as modules.

In the install.inc I only need one thing to make to register the module.

<?php

//We set a version, so we can control if there are any upgrades etc.

$_INSTALL['VERSION'] = 1.07;


//We also create a menu item to be added to the main menu placed in database
$_INSTALL['MAIN_MENU_ITEM'] = array (
    'title' => 'Newsletter', // Title of the menu item
    'url' => '/newsletter/index', // url of the item
    'module_name' => 'newsletter', // beloning to which module
    'parent' => 0, // does it have a parent item
    'weight' => 25, // weight
);

?>

Now we can actually install the module. We do a:

./coscli module --install newsletter

If you go back on your site you can see that the newsletter link is prefixed
with a NT: which means 'Needs Translations'. If you go to anoter link on any
menu you will see that all translations breaks.

In order to fix it we go into the file lang/en_UK/language.inc and adds the
following to the file:

<?php

$_COS_LANG_MODULE = array();
$_COS_LANG_MODULE['Newsletter'] = 'Newsletter';

?>

As you see this will fix most problems. But if you click on another module
menu item you will see that our newsletter menu items breaks. This is because
only the languages for active modules are loaded. There for we need to place
the translation of the menu item in language system table which is loaded on
every page.

We open the file lang/en_UK/system.inc and places the following in it:

<?php

$_COS_LANG_MODULE['Newsletter'] = 'Newsletter';

?>

After that we do a:

./coscli.sh install --reload-language

You will most likely see a bunch of warnings. That is modules without any
translations. Instead of reloading all system translations into database you
can also just uninstall and install the module again, like this.

./coscli.sh module --uninstall newsletter
./coscli.sh module --install newsletter

In short the above means that is you are gonna add site wide translations
add these translations to the system.inc file. If the translation only are
used the current page module is running - then place your translations in
language.inc.

<h1>SQL</h1>

We will need some sql. We will need two tables. One that holds the subscribers,
and a table that holds the newsletters we send. 



