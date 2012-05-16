<form action="" method="post">
<fieldset><legend><?=lang::translate('newsletter_send_newsletter'); ?></legend>
<label for="title"><?=lang::translate('newsletter_title');?></label><br />
<input type="text" name="title" size="30" value="<?=@$_POST['title']?>" />
<br />
<label for="content"><?=lang::translate('newsletter_content')?></label><br />
<textarea name="content" cols="<?=HTML_FORM_TEXTAREA_WT?>" rows="<?=HTML_FORM_TEXTAREA_HT?>" ><?=@$_POST['content']?></textarea>
<label for="submit">&nbsp;</label>
<br />
<label for="test_email"><?=lang::translate('newsletter_test_email');?></label><br />
<input type="text" name="test_email" size="30" value="<?=@$_POST['test_email']?>" />
<br />
<input type="submit" name="submit" value="<?=lang::translate('newsletter_send')?>" />
<br />
</fieldset>

</form>
