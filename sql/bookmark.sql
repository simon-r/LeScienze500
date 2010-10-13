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


create table BookmarkEntries 
(
Id integer primary key autoincrement,
IdEntry integer not null ,
N_Order integer default 1,
expanded boolean default false 
) ;

create table Folders 
(
Id integer primary key autoincrement,
Folder varchar(40) not null,
N_Order integer default 1,
expanded boolean default false 
) ;

create table Folder_SubFolders
(
IdFolder integer not null,
IdSubFolder integer not null unique,
primary key ( IdFolder , IdSubFolder ),
foreign key ( IdFolder ) references Folders( Id ),
foreign key ( IdSubFolder ) references Folders( Id ),
constraint NotSelfReference check ( IdSubFolder <> IdFolder )
) ;

create table UserStates
(
Id integer primary key autoincrement,
StateName varchar(20) not null unique
) ;

create table Comments
(
Id integer primary key autoincrement,
Comment text 
) ;

create table Evaluations
(
Id integer primary key autoincrement,
Evaluation integer unique,
constraint EvaluationsRange check ( Evaluation > 0 and Evaluation <= 5 )
) ;

create table Folders_BookmarkEntries
(
IdFolder integer not null,
IdBookmarkEntry integer not null,
primary key ( IdFolder , IdBookmarkEntry ),
foreign key ( IdFolder ) references Folders( Id ),
foreign key ( IdBookmarkEntry ) references BookmarkEntries( Id )
) ;

create table UserStates_BookmarkEntries
(
IdUserState integer not null,
IdBookmarkEntry integer not null,
primary key( IdUserState, IdBookmarkEntry ),
foreign key( IdUserState ) references UserStates( Id ),
foreign key( IdBookmarkEntry ) references BookmarkEntries( Id )
) ;

create table Comments_BookmarkEntries
(
IdComment integer not null unique,
IdBookmarkEntry integer not null unique,
primary key ( IdComment , IdBookmarkEntry ),
foreign key ( IdComment ) references Comments ( Id ),
foreign key ( IdBookmarkEntry ) references BookmarkEntries ( Id )
) ;

create table Evaluations_BookmarkEntries
(
IdEvaluation integer not null,
IdBookmarkEntry integer not null,
primary key ( IdEvaluation , IdBookmarkEntry ),
foreign key ( IdEvaluation ) references Evaluations ( Id ),
foreign key ( IdBookmarkEntry ) references BookmarkEntries ( Id )
) ;
