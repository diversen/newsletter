DROP TABLE IF EXISTS `newsletter`;

CREATE TABLE `newsletter` (
      `id` int(10) NOT NULL AUTO_INCREMENT,
      `email` varchar(255) NOT NULL,
      `validate_key` varchar(255) NOT NULL,
      `subscribed` boolean DEFAULT 0,
      PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=UTF8;