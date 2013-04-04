<?php

include_once "coslib/captcha.php";
include_once "coslib/pearPager.php";

class newsletter {

    /**
     * 
     * @var array $errors holds errors 
     */
    public static $errors = array();

    /**
     * 
     * @var array $row holds a row 
     */
    public static $row = array();

    /**
     * displays a subscribe form 
     */
    public static function subscribeform (){
        echo view::get('newsletter', 'subscribe_form');
    }

    /**
     *displays a unsubscribe form 
     */
    public static function unsubscribeform (){
        echo view::get('newsletter', 'unsubscribe_form');
    }

    /**
     * validates a subscription. Sets errors in self::$errors if any
     * errors are found. 
     *  
     * @param string $action defaults to subscribe
     *               options are: subscribe | unsubscribe
     * @return  void
     */
    public static function validate ($action = 'subscribe'){
        $db = new db();
        $row = $db->selectOne('newsletter', 'email', $_POST['email']);
        
        if (config::getModuleIni('newsletter_captcha')) {
            if (!session::isUser() && !captcha::checkCaptcha($_POST['captcha'])) {
                self::$errors['captcha'] = lang::translate('system_captcha_error');
            }
        }
        
        if ($action == 'subscribe'){
            if (!empty($row)){
                self::$errors['email_exists'] = lang::translate('newsletter_email_exists');
                return;
            }
        }

        if ($action == 'unsubscribe'){
            if (empty($row)){
                self::$errors['email_not_exists'] = lang::translate('newsletter_email_not_exists');
                return;
            } else {
                self::$row = $row;
            }
        }
        
        

        if (!cosValidate::email($_POST['email'])){
            self::$errors['email'] = lang::translate('newsletter_error_email');
            return;
        }

        if (!cosValidate::validateEmailAndDomain($_POST['email'])){
            self::$errors['email'] = lang::translate('newsletter_error_email');
            return;
        }
        
        
    }

    /**
     * verifies a subscribe in normal web flow. 
     * @return boolean $res true on success and flase on failure
     */
    public function verifySubscriber (){
        
        $db = new db();
        $id = uri::$fragments[2];
        $key = uri::$fragments[3];

        $search = array (
            'id' => $id, 'validate_key' => $key, 'subscribed' => 0
        );
        
        $row = $db->selectOne('newsletter', null, $search);
        //print_r($row);
        //die;
        if ($row['subscribed'] == '0'){

            db::$dbh->beginTransaction();
            $validate_key = md5(uniqid(rand(), true));

            $values = db::prepareToPost();
            $values['validate_key'] = $validate_key;
            $values['subscribed'] = 1;

            $search = array ('id' => $id);

            $res = $db->update('newsletter', $values, $id);
            //$this->update($table, $values, $search, $bind);

            
            if ($res){
                db::$dbh->commit();
                echo lang::translate('newsletter_verify_subscribed');
                return 1;
            } else {
                db::$dbh->rollBack();
                echo lang::translate('newsletter_error_try_later');
                return 0;
            }
        }
        
        $search = array (
            'id' => $id, 'subscribed' => 1
        );
        $row = $db->selectOne('newsletter', null, $search);
        if ($row['subscribed'] == '1'){
            echo lang::translate('newsletter_email_already_subscribed');
            return 0;
        }
    }
    
    /**
     * verifies an unsubscriber
     * @return boolean $res true on success and false on failure 
     */
    public function verifyUnsubscriber (){
        $db = new db();
        
        $id = uri::$fragments[2];
        $key = uri::$fragments[3];

        $search = array (
            'id' => $id, 'validate_key' => $key
        );

        $row = $db->selectOne('newsletter', null, $search);
       
        if (!empty($row)){

            db::$dbh->beginTransaction();

            $res = $db->delete('newsletter', 'id', $id);
            if ($res){
                db::$dbh->commit();
                echo lang::translate('newsletter_verify_unsubscribed');
                return 1;
            } else {
                db::$dbh->rollBack();
                echo lang::translate('newsletter_error_try_later');
                return 0;
            }
        }

        $search = array ('id' => $id);
        
        $row = $db->selectOne('newsletter', null, $search);
        if (empty($row)){
            echo lang::translate('newsletter_email_already_unsubscribed');
            return 0;
        }
    }
    
    public static function updateDirect($row) {
        $db = new db();
        
        $values = array ('subscribed' => 1);
        return $db->update('newsletter', $values, array ('email' => $row['email']));

    }
    
    /**
     * implement events
     * Two types of events are possible so far. 
     * The first event is profile_create, which is used when creating a 
     * profile. It works in conjunction with the other event 'form' 
     * 
     * When using the form event, the event will add a newsletter checkbox
     * to your form with the name 'newsletter'
     * 
     * When the 'newsletter' is checked the events 'profile_create' will 
     * add the current users 'email' to the newsletter table.  
     * 
     * If the 'newsletter' is not checked the email of the user will be deleted
     * from the newsletter table. 
     * 
     * @param array $params array ('user_id' => $id, 'action' => 'the_action');
     * @return boolean $res true on success and false on failure. 
     */
    
    public static function events ($params) {
        
        $news = new newsletter();
        if ($params['action'] == 'profile_update' || $params['action'] == 'profile_create') {
            $res = $news->updateEvent ( $params );
            unset($_POST['newsletter']);
            return $res;        
        }
        
        if ($params['action'] == 'form') {
            $res = $news->formEvent($params);
            return $res;
        }
        
        return true;
    }

    
    /**
     * event for creating a newsletter checkbox for using in other 
     * modules forms 
     * @param array $params 
     */
    public function formEvent ($params) {
        
        $row = array ();
        if (!empty($params['user_id'])) {
            $user = user::getAccount($params['user_id']);
            $row = $this->getSubscriber($user['email']);
        }
        if (!empty($row)) {
            $checked = 1;
        } else {
            $checked = null;
        }
        
        html::label('newsletter', lang::translate('newsletter_form_event_title'));
        html::checkbox('newsletter', $checked);
    }
    
    /**
     * event for adding a user to an email
     * @param array $args
     * @return boolean $res true on success and failure 
     */
    public function updateEvent($args) {
        $db = new db();
        
        if (isset($_POST['newsletter'])) {
            $checked = 1;
        } else {
            $checked = null;
        }

        unset($_POST['newsletter']);
        $user = user::getAccount($args['user_id']);
        $res = $db->delete('newsletter', 'email', $user['email']);
        
        if ($checked) {
            $res = $this->addEmail($user);
        }
        return $res;
        
    }
    
    /**
     * method for adding a email to the newsletter list 
     * @param type $user
     * @return type 
     */
    public function addEmail ($user) {
        $db = new db();
        $values = array ();
        $values['email'] = $user['email'];
        $validate_key = md5(uniqid(rand(), true));
        $values['validate_key'] = $validate_key;
        $values['subscribed'] = 1;
        return $db->insert('newsletter', $values);
    }
    
    /**
     * gets a subscriber from email
     * @param string $email
     * @return array $row 
     */
    public function getSubscriber ($email) {
        $db = new db();
        return $db->selectOne('newsletter', 'email', $email);
    } 
    
    
    /**
     * adds a user from the normal module flow
     * @return boolen $res true on success and false on failure 
     */
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
        $subject = lang::translate('newsletter_subscribe_subject');
        
        // verify url
        $site = $_SERVER['HTTP_HOST'];
        $site_name = 'http://' . $site;
        $subscribe_url = $site_name . "/newsletter/verify/$id/$validate_key";

        $from = config::getMainIni('site_email');
        $reply_to = config::getMainIni('site_email');
        $language = config::getMainIni('language');

        ob_start();
        
        $email_file = _COS_PATH . 
                      '/modules/newsletter/lang/' . 
                      $language . 
                      '/subscribe_email.inc';
        
        include $email_file;
        $message = ob_get_contents();
        ob_end_clean();

        if (cosMail::text($_POST['email'], $subject, $message, $from, $reply_to)){
            db::$dbh->commit();
            return 1;
        } else {
            db::$dbh->rollBack();
            return 0;
        }
    }

    /**
     * method for deleting a subscriber in the normal module web flow
     * @return int 
     */
    public static function deleteSubscriber (){
        $db = new db();
        db::$dbh->beginTransaction();
        $validate_key = md5(uniqid(rand(), true));

        $values = array ();
        $values['validate_key'] = $validate_key;

        $id = self::$row['id'];
        $db->update('newsletter', $values, $id);

        $subject = lang::translate('newsletter_unsubscribe_subject');

        // verify url
        $site = $_SERVER['HTTP_HOST'];
        $site_name = 'http://' . $site;
        $unsubscribe_url = $site_name . "/newsletter/verify_unsubscribe/$id/$validate_key";

        $from = config::getMainIni('site_email');
        $reply_to = config::getMainIni('site_email');
        $language = config::getMainIni('language');

        ob_start();
        $email_file = _COS_PATH . '/modules/newsletter/lang/' . $language . '/unsubscribe_email.inc';
        include $email_file;
        $message = ob_get_contents();
        ob_end_clean();

        if (cosMail::text($_POST['email'], $subject, $message, $from, $reply_to)){
            db::$dbh->commit();
            return 1;
        } else {
            db::$dbh->rollBack();
            return 0;
        }
    }

    /**
     * gets a newsletter
     * @param int $id
     * @return array $row 
     */
    public function getNewsletter($id){
        $db = new db();
        return $db->selectOne('newsletter_archive', 'id', $id);
    }

    /**
     * deletes a newsletter
     * @param int $id
     * @return boolean $res true on success and false on failure 
     */
    public function deleteNewsletter($id){
        $db = new db();
        return $db->delete('newsletter_archive', 'id', $id);
    }

    /**
     * validates send before sending newsletter
     * @return boolean $res true on success and false on failure  
     */
    public static function validateSend (){
        if (isset($_POST['submit'])){
            if (empty($_POST['title']))
                self::$errors[] = lang::translate('newsletter_no_title');
            if (empty($_POST['content']))
                self::$errors[] = lang::translate('newsletter_no_content');
        }

        if (@!empty($_POST['test_email'])){
            if (!cosValidate::email($_POST['test_email'])){
                self::$errors['test_email'] = lang::translate('newsletter_error_test_email');
                return;
            }

            if (!cosValidate::validateEmailAndDomain($_POST['test_email'])){
                self::$errors['test_email'] = lang::translate('newsletter_error_test_email');
                return;
            }
        }
    }

    /**
     * method for sending newsletter in normal web flow. 
     */
    public static function send (){
        ini_set('max_execution_time', 0);

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
        $from = config::getMainIni('site_email');
        $reply_to = config::getMainIni('site_email');
        
        // only a test
        if (!empty($_POST['test_email'])){
            $res['test_email'] = cosMail::text(
                        $_POST['test_email'],
                        $_POST['title'],
                        $_POST['content'],
                        $from, $reply_to);
        } else {
            // send newesletter
            foreach ($rows as $key => $val){
                $res[$val['email']] = cosMail::text(
                        $val['email'],
                        $_POST['title'],
                        $_POST['content'],
                        $from, $reply_to);
            }
            self::addReportToArchive($res, $last_insert_id);
        }
    }

    /**
     * Update newsletter archive with a report. 
     * @param array $res status from all sent newsletters
     * @param int $last_insert_id the last inserted newsletter
     * @return boolean $res true on success and false on failure 
     */
    public static function addReportToArchive($res, $last_insert_id){
        $db = new db();
        $values = array();
        $values['report'] = serialize($res);
        return $db->update('newsletter_archive', $values, $last_insert_id);

    }

    /**
     *displays all sent newsletters 
     */
    public static function displayAllNewsletters (){

        $num_rows = dbQ::setSelectNumRows('newsletter_archive')->fetch();
        $limit = 10;
        $p = new pearPager($num_rows, $limit);
        
        $rows = dbQ::setSelect('newsletter_archive')->order('sent_stamp', 'DESC')->limit($p->from, $limit)->fetch();

        html::headline(lang::translate('newsletter_archive'));
        foreach ($rows as $val){
            $unix_stamp = strtotime($val['sent_stamp']);
            $str = strftime(config::getMainIni('date_format_long'), $unix_stamp);
            $str.= ' ' . $val['title'];
            $str = html::specialEncode($str);
            
            $url = strings::utf8Slug("/newsletter/view/$val[id]", $val['title']);
            echo html::createLink($url, $str);
            echo "<br />\n";
            if (session::isAdmin()){
                echo html::createLink("/newsletter/delete/$val[id]", lang::translate('newsletter_delete_archive_item'));
                echo "<br />\n";
            }
            echo "<hr />\n";
            
        }
        echo $p->getPagerHTML();

    }

    /**
     * method for validating all emails in DB 
     */
    public static function validateAllEmails (){
        ini_set('max_execution_time', 0);
        
        $correct = '';
        $db = new db();
        $rows = $db->selectAll('newsletter');
        $errors = $correct = '';
        $_SESSION['newsletter_email_errors'] = array ();
        foreach ($rows as $key => $val){
            
            if (!cosValidate::validateEmailAndDomain($val['email'])){
                $errors.= lang::translate('newsletter_report_email_error') .  ": " . $val['email'] . "<br />\n";
                $_SESSION['newsletter_email_errors'][] = $val['email'];
            } else {
                $correct.= lang::translate('newsletter_report_email_correct') . ": " . $val['email'] . "<br />\n";
            }
        }


        if (!empty($errors)){
            echo $errors;
            self::displayDeleteCorruptEmails();
        }
        echo "<hr />\n";
        echo $correct;
    }

    /**
     * displays a form for validating all newsletters 
     */
    public static function displayValidateAllEmails (){
        html::confirm(lang::translate('newsletter_validate_all_message'));
        echo "<form method =\"post\" action= \"\">\n";
        echo "<input type = \"submit\" name=\"submit\" value =\"" .lang::translate('newsletter_validate_all') . "\" />";
        echo "</form>\n";
    }

    /**
     * dislays a confirm form for delting corrupt emails 
     */
    public static function displayDeleteCorruptEmails (){
        echo "<form method =\"post\" action= \"\">\n";
        echo "<input type = \"submit\" name=\"delete\" value =\"" .lang::translate('newsletter_delete_broken_emails') . "\" />";
        echo "</form>\n";
    }

    /**
     * method for delting broken emails.
     * @return boolean $res true on success and false on failure  
     */
    public static function deleteBrokenEmails(){
        $db = new db();
        foreach ($_SESSION['newsletter_email_errors'] as $key => $val){
            return $db->delete('newsletter', 'email', $val);
        }
    }
}
