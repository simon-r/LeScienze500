insert into Stato ( Stato ) values ( "Da leggere" ) ;
insert into Stato ( Stato ) values ( "Parzialmente letto" ) ;
insert into Stato ( Stato ) values ( "Letto" ) ;

insert into Valutazioni ( Valutazione ) values ( 1 ) ;
insert into Valutazioni ( Valutazione ) values ( 2 ) ;
insert into Valutazioni ( Valutazione ) values ( 3 ) ;
insert into Valutazioni ( Valutazione ) values ( 4 ) ;
insert into Valutazioni ( Valutazione ) values ( 5 ) ;

insert into Categorie ( Categoria ) values ( "Favoriti" ) ;

-- test:
insert into Categorie ( Categoria ) values ( "Mio" ) ;
insert into Categoria_SottoCategoria ( IdCategoria , IdSottoCategoria ) values ( 1 , 2 ) ;

insert into Categorie ( Categoria ) values ( "Mio_2" ) ;
insert into Categoria_SottoCategoria ( IdCategoria , IdSottoCategoria ) values ( 1 , 3 ) ;

insert into Categorie ( Categoria ) values ( "Mio_3" ) ;
insert into Categoria_SottoCategoria ( IdCategoria , IdSottoCategoria ) values ( 1 , 4 ) ;

insert into Categorie ( Categoria ) values ( "sotto" ) ;
insert into Categoria_SottoCategoria ( IdCategoria , IdSottoCategoria ) values ( 2 , 5 ) ;

insert into Categorie ( Categoria ) values ( "sotto2" ) ;
insert into Categoria_SottoCategoria ( IdCategoria , IdSottoCategoria ) values ( 2 , 6 ) ;

insert into Categorie ( Categoria ) values ( "Preferiti" ) ;

insert into Favoriti ( IdArticolo ) values ( 1 ) ;
insert into Favoriti ( IdArticolo ) values ( 13 ) ;
insert into Favoriti ( IdArticolo ) values ( 2000 ) ;
insert into Favoriti ( IdArticolo ) values ( 3333 ) ;


insert into Categorie_Favoriti ( IdCategoria , IdFavorito ) values ( 3 , 3 ) ;
insert into Categorie_Favoriti ( IdCategoria , IdFavorito ) values ( 4 , 3 ) ;
insert into Categorie_Favoriti ( IdCategoria , IdFavorito ) values ( 1 , 6 ) ;
