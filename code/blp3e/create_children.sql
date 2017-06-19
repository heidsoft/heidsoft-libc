

CREATE TABLE children (
   childno int(11) DEFAULT '0' NOT NULL auto_increment,
   fname varchar(30),
   age int(11),
   PRIMARY KEY (childno)
);

--
-- Dumping data for table 'children'
--

INSERT INTO children VALUES (1,'Jenny',17);
INSERT INTO children VALUES (2,'Andrew',13);
INSERT INTO children VALUES (3,'Gavin',4);
INSERT INTO children VALUES (4,'Duncan',2);
INSERT INTO children VALUES (5,'Emma',0);
INSERT INTO children VALUES (6,'Alex',11);
INSERT INTO children VALUES (7,'Adrian',5);

