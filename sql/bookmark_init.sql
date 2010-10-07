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