DROP TABLE IF EXISTS `newsletter_archive`;

CREATE TABLE `newsletter_archive` (
      `id` int(10) NOT NULL AUTO_INCREMENT,
      `title` varchar(255) NOT NULL,
      `content` text NOT NULL,
      `sent_stamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
      PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=UTF8;