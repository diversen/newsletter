<form action="" method="post">
<fieldset><legend><?=lang::translate('newsletter_add_direct'); ?></legend>
<label for="test_email"><?=lang::translate('newsletter_form_email');?></label><br />
<input type="text" name="email" size="30" value="<?=@$_POST['email']?>" />
<br />
<label for="submit">&nbsp;</label>
<br />
<input type="submit" name="submit" value="<?=lang::translate('send')?>" />
<br />
</fieldset>
</form>