<form action="" method="post">
<fieldset><legend><?=lang::translate('send_newsletter'); ?></legend>
<label for="title"><?=lang::translate('title');?></label><br />
<input type="text" name="title" size="30" value="<?=@$_POST['title']?>" />
<br />
<label for="content"><?=lang::translate('content')?></label><br />
<textarea name="content" cols="60" rows="20" ><?=@$_POST['content']?></textarea>
<label for="submit">&nbsp;</label>
<br />
<label for="test_email"><?=lang::translate('test_email');?></label><br />
<input type="text" name="test_email" size="30" value="<?=@$_POST['test_email']?>" />
<br />
<input type="submit" name="submit" value="<?=lang::translate('send')?>" />
<br />
</fieldset>

</form>
