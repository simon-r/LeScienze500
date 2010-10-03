create table Favoriti 
(
	Id integer primary key autoincrement,
	IdArticolo integer not null unique
) ;

create table Categorie 
(
	Id integer primary key autoincrement,
	Categoria varchar(40) not null unique
) ;

create table Categoria_SottoCategoria
(
	IdCategoria integer not null,
	IdSottoCategoria integer not null,
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
	IdCategorie integer not null,
	IdFavoriti integer not null,
	primary key ( IdCategorie , IdFavoriti ),
	foreign key ( IdCategorie ) references Categorie( Id ),
	foreign key ( IdFavoriti ) references Favoriti( Id )
) ;

create table Stato_Favoriti 
(
	IdStato integer not null,
	IdFavoriti integer not null,
	primary key( IdStato, IdFavoriti ),
	foreign key( IdStato ) references Stato( Id ),
	foreign key( IdFavoriti ) references Favoriti( Id )
) ;

create table Commento_Favoriti
(
	IdCommento integer not null,
	IdFavoriti integer not null,
	primary key ( IdCommento , IdFavoriti ),
	foreign key ( IdCommento ) references Commenti ( Id ),
	foreign key ( IdFavoriti ) references Favoriti ( Id )
) ;

create table Valutazioni_Favoriti
(
	IdValutazioni integer not null,
	IdFavoriti integer not null,
	primary key ( IdValutazioni , IdFavoriti ),
	foreign key ( IdValutazioni ) references Valutazioni ( Id ),
	foreign key ( IdFavoriti ) references Favoriti ( Id )
) ;
