CREATE TABLE Categorie 
(
	Id int NOT NULL PRIMARY KEY,
	Categoria nvarchar(50) NOT NULL
) ;


CREATE TABLE Rubriche 
(
    Id int PRIMARY KEY NOT NULL,
    Rubrica varchar(50) NOT NULL
) ;

CREATE TABLE Articoli 
(
	Id int NOT NULL PRIMARY KEY,
	IdRivista int NOT NULL,
	IdRubrica int NOT NULL ,
	Titolo varchar(150) NOT NULL,
	Abstract varchar(4000),
	FilePDF varchar(50) NOT NULL,
	FOREIGN KEY (IdRubrica) REFERENCES Rubriche(Id)
) ;
	
CREATE TABLE RicercaEstesa 
(
    IdArticolo int PRIMARY KEY NOT NULL,
    TestoEsteso text character set utf8,
    FOREIGN KEY (IdArticolo) REFERENCES Articoli(Id)
) ;

CREATE TABLE Autori 
(
	Id int NOT NULL PRIMARY KEY,
	Autore varchar(100),
	Cognome varchar(30) character set utf8,
	Nome varchar(30) character set utf8
) ;
		
CREATE TABLE Articoli_Categorie
(
	IdArticolo int NOT NULL ,
	IdCategoria int NOT NULL,
	PRIMARY KEY ( IdArticolo, IdCategoria ),
	FOREIGN KEY (IdArticolo) REFERENCES Articoli(Id),
	FOREIGN KEY (IdCategoria) REFERENCES Categoria(Id)
	
) ;


CREATE TABLE Articoli_Autori
(
	IdArticolo int NOT NULL ,
	IdAutore int NOT NULL ,
	PRIMARY KEY ( IdArticolo, IdAutore ),
	FOREIGN KEY (IdArticolo) REFERENCES Articoli(Id) ,
	FOREIGN KEY (IdAutore) REFERENCES Autori(Id)
) ;

