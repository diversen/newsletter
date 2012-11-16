<form method ="post" action ="/newsletter/unsubscribe">
<fieldset>
<legend><?=lang::translate('newsletter_unsubscribe_to_newsletter')?></legend>

<?php


html::init(null, 'submit');
$options = array ('required' => true);
echo html::labelClean('email', lang::translate('email'), $options);
echo html::textClean('email', @htmlentities($_POST['email']));

if (config::getModuleIni('newsletter_captcha')) {
    echo html::labelClean('captcha', captcha::createCaptcha());
    echo html::textClean('captcha');
}


?>
<label for="submit"></label><br />
<input type="submit" name="submit" value="<?=lang::translate('newsletter_unsubscribe')?>"><br />
</fieldset>
</form>