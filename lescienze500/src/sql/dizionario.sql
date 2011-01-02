create table Words
(
	Id integer primary key autoincrement,
	Word nvarchar(150) not null,
	Cnt integer not null default 1
) ;


create table Categorie 
(
	Id integer primary key autoincrement,
	Categoria nvarchar(50) not null
) ;

create table Articoli
(
	Id integer primary key autoincrement,
	IdArticolo integer not null 
) ;

create table Anni
(
	Id integer primary key autoincrement,
	Anno integer not null unique
) ;

create table Word_Categoria
(
	IdWord integer not null,
	IdCategoria integer not null,
	Cnt integer not null default 1,
	primary key ( IdWord , IdCategoria )
) ;

create table Word_Articolo
(
	IdWord integer not null unique,
	IdArticolo integer not null unique,
	Cnt integer not null default 1 ,
	primary key ( IdWord , IdArticolo )
	
) ;

create table Word_Anno
(
	IdWord integer not null,
	Anno integer not null,
	Cnt integer not null default 1 ,
	primary key ( IdWord , Anno )
) ;
