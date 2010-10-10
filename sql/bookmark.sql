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


create table Favoriti 
(
	Id integer primary key autoincrement,
	IdArticolo integer not null unique,
	Ordine integer
) ;

create table Categorie 
(
	Id integer primary key autoincrement,
	Categoria varchar(40) not null,
	Ordine integer
) ;

create table Categoria_SottoCategoria
(
	IdCategoria integer not null,
	IdSottoCategoria integer not null unique,
	primary key ( IdCategoria , IdSottoCategoria ),
	foreign key ( IdCategoria ) references Categorie( Id ),
	foreign key ( IdSottoCategoria ) references Categorie( Id ),
	constraint NonSelfReference check ( IdSottoCategoria <> IdCategoria )
) ;

create table Stato
(
	Id integer primary key autoincrement,
	Stato varchar(20) not null unique
) ;

create table Commenti
(
	Id integer primary key autoincrement,
	Commento text 
) ;

create table Valutazioni
(
	Id integer primary key autoincrement,
	Valutazione integer unique,
	constraint RangeValutazioni check ( Valutazione > 0 and Valutazione <= 5 )
) ;

create table Categorie_Favoriti
(
	IdCategoria integer not null,
	IdFavorito integer not null,
	primary key ( IdCategoria , IdFavorito ),
	foreign key ( IdCategoria ) references Categorie( Id ),
	foreign key ( IdFavorito ) references Favoriti( Id )
) ;

create table Stato_Favoriti 
(
	IdStato integer not null,
	IdFavorito integer not null,
	primary key( IdStato, IdFavoriti ),
	foreign key( IdStato ) references Stato( Id ),
	foreign key( IdFavorito ) references Favoriti( Id )
) ;

create table Commento_Favoriti
(
	IdCommento integer not null unique,
	IdFavorito integer not null unique,
	primary key ( IdCommento , IdFavoriti ),
	foreign key ( IdCommento ) references Commenti ( Id ),
	foreign key ( IdFavorito ) references Favoriti ( Id )
) ;

create table Valutazioni_Favoriti
(
	IdValutazione integer not null,
	IdFavorito integer not null,
	primary key ( IdValutazione , IdFavorito ),
	foreign key ( IdValutazione ) references Valutazioni ( Id ),
	foreign key ( IdFavorito ) references Favoriti ( Id )
) ;
