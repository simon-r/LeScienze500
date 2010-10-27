--     Copyright 2010 Simone Riva
--     mail:  simone (dot) rva (lumachina) gmail.com
--
--    This file is part of LeScienze500.
--
--    LeScienze500 is free software: you can redistribute it and/or modify
--    it under the terms of the GNU General Public License as published by
--    the Free Software Foundation, either version 3 of the License, or
--    (at your option) any later version.
--
--    LeScienze500 is distributed in the hope that it will be useful,
--    but WITHOUT ANY WARRANTY; without even the implied warranty of
--    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--    GNU General Public License for more details.
--
--    You should have received a copy of the GNU General Public License
--    along with LeScienze500.  If not, see <http://www.gnu.org/licenses/>.


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

