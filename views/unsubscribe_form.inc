<form method ="post" action ="/newsletter/unsubscribe">
<fieldset>
<legend><?=lang::translate('newsletter_unsubscribe_to_newsletter')?></legend>
<label for="email">Email:</label><br />
<input type="text" name="email" size ="30" value="<?=@htmlentities($_POST['email'])?>"><br />
<label for="submit"></label><br />
<input type="submit" name="submit" value="<?=lang::translate('newsletter_unsubscribe')?>"><br />
</fieldset>
</form>