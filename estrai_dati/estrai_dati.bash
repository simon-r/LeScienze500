#bash 

echo ""
echo " ------------------------------------------------------------ "
echo " lescienze500 data - ATTENZIONE - "
echo ""
echo " Tutti i files estratti dall'archivio install.dmg sono "
echo " protetti da copyright e propieta di:  "
echo ""
echo " LE SCIENZE S.p.A.  "
echo " Sede sociale Via Cristoforo Colombo, 149 - 00147 Roma  "
echo " Capitale Sociale Euro 103.400,00 i.v.  "
echo " R.E.A. Roma n. 1038183 - Codice fiscale e Iscriz. Registro Imprese di Roma 00882050156  "
echo " Partita IVA: 00882050156 "
echo ""
echo " I files generati con questa procedura devono essere usati "
echo " a titolo esclusivamente privato. "
echo " La distribuzione mediante qualsiasi mezzo anche parziale dei files generati da questo script è VIETATA "
echo ""
echo " L'autore di questo script non fornirà in nessun modo il file install.dmg"
echo ""
echo " Ho letto l'avviso e accetto le condizioni (si) (no) "

read  rispo

if [ $rispo != "si"  ] ; then
	echo "Non hai accettato le condizioni; procedura terminata"
	exit 1
fi


if [ -e install.dmg ] ; then 
	ARCHLS=install.dmb
elif [ -e  /media/LESCIENZE/install.dmg ] ; then
	ARCHLS=/media/LESCIENZE/install.dmg
else
	echo "ipossibile trovare il file install.dmg"
	echo "copiare il file nella directory corrente" 
	echo "oppure inserire il DVD 1 e montarlo in:"
	echo "/media/LESCIENZE"
	exit 1
fi

echo " Directory di destinazione delle copertine e del database ($HOME/lescieneze500) "

read dest_path
if [ $dest_path != "\n" ] ; then
	echo "detinazione: $dest_path"
else
	dest_path=$HOME/lescieneze500-data
	echo "detinazione: $dest_path"
fi


mkdir -p $dest_path

7z -o/tmp -y x $ARCHLS 2.hfs
7z -o$dest_path -y x /tmp/2.hfs Le\ Scienze\ -\ 500\ Numeri/Le\ Scienze\ -\ 500\ Numeri/copertine/
7z -o$dest_path -y x /tmp/2.hfs Le\ Scienze\ -\ 500\ Numeri/Le\ Scienze\ -\ 500\ Numeri/data/LeScienze.db
rm /tmp/2.hfs

mv $dest_path/Le\ Scienze\ -\ 500\ Numeri/Le\ Scienze\ -\ 500\ Numeri/copertine/ $dest_path/copertine
mkdir -p $dest_path/data
mv $dest_path/Le\ Scienze\ -\ 500\ Numeri/Le\ Scienze\ -\ 500\ Numeri/data/LeScienze.db $dest_path/data

rm -r $dest_path/Le\ Scienze\ -\ 500\ Numeri/


