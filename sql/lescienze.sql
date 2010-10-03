CREATE TABLE Categorie 
(
	Id integer PRIMARY KEY NOT NULL AUTO_INCREMENT,
	Categoria nvarchar(50) NOT NULL
) ;


CREATE TABLE Rubriche 
(
	Id integer PRIMARY KEY NOT NULL AUTO_INCREMENT,
	Rubrica varchar(50) NOT NULL
) ;

CREATE TABLE Riviste
(
	Id integer PRIMARY KEY NOT NULL AUTO_INCREMENT,       
	Numero integer NOT NULL,
	Anno integer NOT NULL,
	Mese varchar(15) NOT NULL,
	FileCopertina varchar(15) NOT NULL
);

CREATE TABLE Articoli 
(
	Id integer PRIMARY KEY NOT NULL AUTO_INCREMENT,
	IdRivista int NOT NULL,
	IdRubrica int NOT NULL ,
	Titolo varchar(150) NOT NULL,
	Abstract varchar(4000),
	FilePDF varchar(50) NOT NULL,
	FOREIGN KEY (IdRubrica) REFERENCES Rubriche(Id),
	FOREIGN KEY (IdRivista) REFERENCES Riviste(Id)
) ;
	
CREATE TABLE RicercaEstesa 
(
	IdArticolo integer PRIMARY KEY NOT NULL AUTO_INCREMENT,
	TestoEsteso text character set utf8,
	FOREIGN KEY (IdArticolo) REFERENCES Articoli(Id)
) ;

CREATE TABLE Autori 
(
	Id integer PRIMARY KEY NOT NULL AUTO_INCREMENT,
	Autore varchar(100),
	Cognome varchar(30) character set utf8,
	Nome varchar(30) character set utf8
) ;
		
CREATE TABLE Articoli_Categorie
(
	IdArticolo integer NOT NULL ,
	IdCategoria integer NOT NULL,
	PRIMARY KEY ( IdArticolo, IdCategoria ),
	FOREIGN KEY (IdArticolo) REFERENCES Articoli(Id),
	FOREIGN KEY (IdCategoria) REFERENCES Categoria(Id)
) ;


CREATE TABLE Articoli_Autori
(
	IdArticolo integer NOT NULL ,
	IdAutore integer NOT NULL ,
	PRIMARY KEY ( IdArticolo, IdAutore ),
	FOREIGN KEY (IdArticolo) REFERENCES Articoli(Id) ,
	FOREIGN KEY (IdAutore) REFERENCES Autori(Id)
) ;

