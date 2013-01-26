<?php

$id = uri::getInstance()->fragment(2);
if (!$id) {
    http::permMovedHeader('/newsletter/archive');
}

$news = new newsletter();
$row = $news->getNewsletter($id);

$url = strings::utf8Slug("/newsletter/view/$row[id]", $row['title']);
http::permMovedHeader($url);

if (!empty($row)){

    $filters = config::getModuleIni('newsletter_filters');
    $content =moduleloader::getFilteredContent($filters, $row['content']);
    echo "<h3 id=\"start\">" . html::specialEncode($row['title']) . "</h3>";
    echo $content;
    $title = lang::translate('newsletter_archive_html_title_item');
    $title.= MENU_SUB_SEPARATOR_SEC . html::specialEncode($row['title']);
    template::setTitle($title);
    template::setMeta(array('description' => strings::substr2(html::specialEncode($row['content']), 155, 3)));
} else {
    template::setTitle(lang::translate('newsletter_archive_html_title'));
    template::setMeta(array('description' => lang::translate('newsletter_archive_index_meta')));
}
