<?php

class newsletter extends db {

    public static $errors = array();

    public static $row = array();

    public static function subscribeform (){
    
    ?>

<form method ="post" action ="/newsletter/subscribe">
<fieldset>
<legend><?=lang::translate('subscribe_to_newsletter')?></legend>
<label for="email">Email:</label><br />
<input type="text" name="email" size ="30" value="<?=@htmlentities($_POST['email']);?>"><br />
<label for="submit"></label><br />
<input type="submit" name="submit" value="<?=lang::translate('subscribe')?>"><br />
</fieldset>
</form>
<?php
 
    }

public static function unsubscribeform (){

    ?>

<form method ="post" action ="/newsletter/unsubscribe">
<fieldset>
<legend><?=lang::translate('unsubscribe_to_newsletter')?></legend>
<label for="email">Email:</label><br />
<input type="text" name="email" size ="30" value="<?=@htmlentities($_POST['email'])?>"><br />
<label for="submit"></label><br />
<input type="submit" name="submit" value="<?=lang::translate('unsubscribe')?>"><br />
</fieldset>
</form>
<?php

    }

    public static function validate ($action = 'subscribe'){
        $db = new db();
        $row = $db->selectOne('newsletter', 'email', $_POST['email']);
        if ($action == 'subscribe'){
            if (!empty($row)){
                self::$errors['email_exists'] = lang::translate('email_exists');
                return;
            }
        }

        if ($action == 'unsubscribe'){
            if (empty($row)){
                self::$errors['email_not_exists'] = lang::translate('email_not_exists');
                return;
            } else {
                self::$row = $row;
            }
        }

        if (!cosValidate::email($_POST['email'])){
            self::$errors['email'] = lang::translate('error_email');
            return;
        }

        if (!cosValidate::validateEmailAndDomain($_POST['email'])){
            self::$errors['email'] = lang::translate('error_email');
            return;
        }

    }

    public function verifySubscriber (){
        $id = uri::$fragments[2];
        $key = uri::$fragments[3];

        $search = array (
            'id' => $id, 'validate_key' => $key, 'subscribed' => 0
        );
        
        $row = $this->selectOne('newsletter', null, $search);
        if ($row['subscribed'] == '0'){

            db::$dbh->beginTransaction();
            $validate_key = md5(uniqid(rand(), true));

            $values = db::prepareToPost();
            $values['validate_key'] = $validate_key;
            $values['subscribed'] = 1;

            $search = array ('id' => $id);

            $res = $this->update('newsletter', $values, $id);
            //$this->update($table, $values, $search, $bind);
            if ($res){
                db::$dbh->commit();
                echo lang::translate('verify_subscribed');
                return 1;
            } else {
                db::$dbh->rollBack();
                echo lang::translate('error_try_later');
                return 0;
            }
        }
        
        $search = array (
            'id' => $id, 'subscribed' => 1
        );
        $row = $this->selectOne('newsletter', null, $search);
        if ($row['subscribed'] == '1'){
            echo lang::translate('email_already_subscribed');
            return 0;
        }
    }

    public function verifyUnsubscriber (){
        $id = uri::$fragments[2];
        $key = uri::$fragments[3];

        $search = array (
            'id' => $id, 'validate_key' => $key
        );

        $row = $this->selectOne('newsletter', null, $search);
       
        if (!empty($row)){

            db::$dbh->beginTransaction();

            $res = $this->delete('newsletter', 'id', $id);
            if ($res){
                db::$dbh->commit();
                echo lang::translate('verify_unsubscribed');
                return 1;
            } else {
                db::$dbh->rollBack();
                echo lang::translate('error_try_later');
                return 0;
            }
        }

        $search = array (
            'id' => $id,
        );
        $row = $this->selectOne('newsletter', null, $search);
        if (empty($row)){
            echo lang::translate('email_already_unsubscribed');
            return 0;
        }
    }

    public static function addSubscriber (){
        $db = new db ();
        db::$dbh->beginTransaction();
        $validate_key = md5(uniqid(rand(), true));

        $values = db::prepareToPost();
        $values['validate_key'] = $validate_key;
        $values['subscribed'] = 0;
        $db->insert('newsletter', $values);

        $id = db::$dbh->lastInsertId();

        // subject
        $subject = lang::translate('subscribe_subject');
        
        // verify url
        $site = $_SERVER['HTTP_HOST'];
        $site_name = 'http://' . $site;
        $subscribe_url = $site_name . "/newsletter/verify/$id/$validate_key";

        // from
        $from = get_main_ini('site_email');
        
        // reply-to
        $reply_to = get_module_ini('newsletter_reply_to');
        
        // language
        $language = get_main_ini('language');

        ob_start();
        $email_file = _COS_PATH . '/modules/newsletter/lang/' . $language . '/subscribe_email.php';
        include $email_file;
        $message = ob_get_contents();
        ob_end_clean();
        //return 1;

        if (mail_utf8($_POST['email'], $subject, $message, $from, $reply_to)){
            db::$dbh->commit();
            return 1;
        } else {
            db::$dbh->rollBack();
            return 0;
        }
    }


    public static function deleteSubscriber (){
        $db = new db();
        db::$dbh->beginTransaction();
        $validate_key = md5(uniqid(rand(), true));

        //$values = db::prepareToPost();
        $values['validate_key'] = $validate_key;

        $n_id = self::$row['id'];
        $db->update('newsletter', $values, $n_id);

        //$id = db::$dbh->lastInsertId();

        // subject
        $subject = lang::translate('unsubscribe_subject');

        // verify url
        $site = $_SERVER['HTTP_HOST'];
        $site_name = 'http://' . $site;
        $unsubscribe_url = $site_name . "/newsletter/verify_unsubscribe/$n_id/$validate_key";

        // from
        $from = get_main_ini('site_email');

        // reply-to
        $reply_to = get_module_ini('newsletter_reply_to');

        // language
        $language = get_main_ini('language');

        ob_start();
        $email_file = _COS_PATH . '/modules/newsletter/lang/' . $language . '/unsubscribe_email.php';
        include $email_file;
        $message = ob_get_contents();
        ob_end_clean();
        //return 1;

        if (mail_utf8($_POST['email'], $subject, $message, $from, $reply_to)){
            db::$dbh->commit();
            return 1;
        } else {
            db::$dbh->rollBack();
            return 0;
        }
    }

    public static function validateSend (){
        if (isset($_POST['submit'])){
            if (empty($_POST['title']))
                self::$errors[] = lang::translate('No title');
            if (empty($_POST['content']))
                self::$errors[] = lang::translate('No content');
        }

        if (@!empty($_POST['test_email'])){
            if (!cosValidate::email($_POST['test_email'])){
                self::$errors['test_email'] = lang::translate('error_test_email');
                return;
            }

            if (!cosValidate::validateEmailAndDomain($_POST['test_email'])){
                self::$errors['test_email'] = lang::translate('error_test_email');
                return;
            }
        }
    }

    public static function send (){
        ini_set('max_execution_time', 0);
        //echo ini_get('max_execution_time');
        // only store in db if not testing
        if (empty($_POST['test_email'])){
            $db = new db();
            db::$dbh->beginTransaction();
            $values = db::prepareToPost();
            unset($values['test_email']);
            $res = $db->insert('newsletter_archive' , $values);

            $last_insert_id = self::$dbh->lastInsertId();

            db::$dbh->commit();
            $rows = $db->selectAll('newsletter', null, array ('subscribed' => '1'));
        }

        // create results ary for status report
        $res = array ();

        // from
        $from = get_main_ini('site_email');

        // reply-to
        $reply_to = get_module_ini('newsletter_reply_to');
        
        // only a test
        if (!empty($_POST['test_email'])){
            $res['test_email'] = mail_utf8(
                        $_POST['test_email'],
                        $_POST['title'],
                        $_POST['content'],
                        $from, $reply_to);
        } else {
            // send newesletter
            foreach ($rows as $key => $val){
                $res[$val['email']] = mail_utf8(
                        $val['email'],
                        $_POST['title'],
                        $_POST['content'],
                        $from, $reply_to);
            }
            self::generateReport($res, $last_insert_id);
        }
    }

    public static function generateReport($res, $last_insert_id){
        $db = new db();
        $values = array();
        $values['report'] = serialize($res);
        //$search = array ('id' => $last_insert_id);
        return $db->update('newsletter_archive', $values, $last_insert_id);

    }

    public static function viewReports (){
        $db = new db();
        $rows = $db->selectAll('newsletter_archive');
        //print_r($rows);
        $newsletter = array();
        foreach ($rows as $key => $val){
            $unix_stamp = strtotime($val['sent_stamp']);
            $str = strftime(get_main_ini('date_format_long'), $unix_stamp);
            $str.= ' ' . $val['title'];
            echo html::createLink("/newsletter/archive?id=$val[id]#start", $str);
            echo "<br />\n";
            if (session::isAdmin()){
                echo html::createLink("/newsletter/archive/$val[id]/delete", lang::translate('newsletter_delete_archive_item'));
                echo "<br />\n";
            }
            echo "<hr />\n";
            if (isset($_GET['id'])) {
                if ($val['id'] == $_GET['id']){
                    $newsletter = $val;
                }
            }
        }
        if (!empty($newsletter)){
            echo $newsletter['content'];
        }
    }

    public static function validateAllEmails (){
        ini_set('max_execution_time', 0);
        $db = new db();
        $rows = $db->selectAll('newsletter');
        $errors = $correct = '';
        $_SESSION['newsletter_email_errors'] = array ();
        foreach ($rows as $key => $val){
            
            if (!cosValidate::validateEmailAndDomain($val['email'])){
                //self::$errors['test_email'] = lang::translate('error_test_email');
                $errors.= lang::translate('report_email_error') .  ": " . $val['email'] . "<br />\n";
                $_SESSION['newsletter_email_errors'][] = $val['email'];
            } else {
                $correct.= lang::translate('report_email_correct') . ": " . $val['email'] . "<br />\n";
            }
        }


        if (!empty($errors)){
            echo $errors;
            self::validateDeleteEmailsForm();
        }
        echo "<hr />\n";
        echo $correct;
    }

    public static function validateAlllEmailsForm (){
        view_confirm(lang::translate('validate_all_message'));
        echo "<form method =\"post\" action= \"\">\n";
        echo "<input type = \"submit\" name=\"submit\" value =\"" .lang::translate('validate_all') . "\" />";
        echo "</form>\n";
    }

    public static function validateDeleteEmailsForm (){
        echo "<form method =\"post\" action= \"\">\n";
        echo "<input type = \"submit\" name=\"delete\" value =\"" .lang::translate('delete_broken_emails') . "\" />";
        echo "</form>\n";
    }

    public static function deleteBrokenEmails(){
        $db = new db();
        foreach ($_SESSION['newsletter_email_errors'] as $key => $val){
            $db->delete('newsletter', 'email', $val);
        }
    }
}