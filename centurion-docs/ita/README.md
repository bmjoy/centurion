# Imper4m Project - Workflow

## Gameplay

1. Il gioco ha un sistema di rendering 3D, anche se la visuale sarà impostata a 2.5D, come *Age of Empires*. La telecamera è in alto, con proiezione ortogonale e non è regolabile né in rotazione né in zoom.

2. Poiché i soldati, così come le strutture, sono dei modelli 3D, è necessario imporre un limite massimo di reclutamento, giacché più modelli sono visualizzati in un'unica videata e maggiori saranno i poligoni da processare e da renderizzare (maggiore quantità di calcolo significa calo delle prestazioni in termini di FPS). Tale limitazione è applicata a ogni giocatore, che non potrà avere più di 250 truppe (comprendendo con questo termine sia quelle di terra sia quelle di mare): essendoci al massimo 8 giocatori per partita, si avrà un totale di 2.000 soldati disponibili contemporaneamente.

3. Escluso il legno, l'argilla (con cui sono fatte le tegole) e qualche altro materiale residuo, le unità e le strutture sono composte da diversi strati, il cui colore varia al variare di certe condizioni.
    * **Materiale assegnato alle parti metalliche** : valido solo per le truppe militari e le navi, include tutte le parti il cui colore dipende dal livello dell'unità presa in esame. Dall'uno al cinque abbiamo un colore bianco scuro/grigio spento; dal sei all'undici abbiamo un colore bianco acceso/grigio lucente; dal dodici al diciassette abbiamo un colore bronzeo; dal diciotto fino al mille abbiamo un colore dorato;
    * **Materiale assegnato alle decorazioni** : valido sia per le unità sia per le strutture, include tutte le parti molto visibili (come ad esempio dei drappi, dei mantelli o delle parti di vestiti) la cui colorazione dipende dal colore del giocatore che li controlla.
4. Il sistema di selezione è identico a quello di un comune RTS: facendo un singolo click si può selezionare un'unità o un edificio, mentre tenendo premuto il tasto sinistro, si disegna un'area rettangolare in cui saranno inserite solamente tutte le unità all'interno del rettangolo, ma non gli edifici. Avendo selezionato un edificio o una o più unità, si possono creare dei gruppi di selezione rapida tenendo premuto il tasto CTRL e premendo i tasti numerici dall'uno al nove. Questo permette al giocatore che preme il numero del gruppo cui sono stati assegnati gli oggetti desiderati, di selezionarli automaticamente senza la necessità di essere costretto a cercarli in mappa.

5. Nel menù principale è possibile avviare una partita scegliendo quella che sarà la modalità di gioco.
    * **Avventura:** È una modalità in cui tutti i giocatori non possono né vincere né essere sconfitti alle normali condizioni, e il cui scopo è quello di utilizzare le risorse a disposizione per completare gli obiettivi. Il comportamento della mappa o delle mappe è gestito da delle regole di gioco, le quali, al verificarsi di determinate condizioni, producono tutta una serie di conseguenze ed eventi.
    * **Conquista**. È una modalità il cui scopo è quello di conquistare tutti i territori di una regione (più o meno grande, come l'Europa ad esempio), scegliendo esclusivamente il popolo con cui giocare, il territorio da occupare (che deve essere necessariamente confinante con i possedimenti di proprio dominio) e il livello di difficoltà. Ogni civiltà avrà un bonus specifico, che le consentirà di ottenere un vantaggio rispetto alle altre; tuttavia, ogniqualvolta viene occupato un nuovo territorio, il bonus della civiltà di partenza sarà rimpiazzato da quello della civiltà conquistata. (**Roma** : 6.000 pezzi d'oro aggiuntivi e un eroe al livello 20; **Gallia** : 4 guerrieri di Fand di livello 12. Gli eroi e i guerrieri acquisiranno il doppio dell'esperienza durante la battaglia; **Iberia** : i guerrieri guariranno quando la loro energia sarà al massimo. Gli eroi e i guerrieri guadagneranno esperienza col passare del tempo; **Cartagine** : 4 Elefanti da guerra. I guerrieri che sconfiggono un nemico ottengono l'oggetto "Bottino di guerra"; **Egitto** : gli eserciti sopravvivranno senza alimentarsi. Quando entreranno in un foro, gli eroi e i guerrieri riceveranno tutti i talismani degli dei; **Britannia** : il prossimo territorio che da conquistare sarà già esplorato e non ci sarà nebbia di guerra. A inizio partita saranno collocati 30 arcieri nel foro, e si rigenereranno ogniqualvolta muoiano; **Germania** : i tuoi guerrieri riceveranno l'abilità speciale "Istruzione". Tutti i fori in possesso produrranno costantemente una quantità extra di viveri);
    * **Schermaglia**. È una modalità che consente la personalizzazione in ogni aspetto; in particolare è possibile stabilire:
        * il tipo di mappa in cui avviare la partita, che potrà essere generata casualmente (isola, continente, arcipelago e così via) o scelta da un elenco di mappe create con l'editor;
        * la stagione in cui giocare;
        * se la mappa è esplorata;
        * se non c'è la nebbia di guerra;
        * se la popolazione negli insediamenti è poca, discreta o molta;
        * se le risorse all'interno degli insediamenti sono poche, medie o molte;
        * quanti giocatori giocheranno, quale sarà la loro civiltà e il loro colore, quali di loro saranno controllati dall'IA, quale sarà la difficoltà con cui si impegneranno a vincere e quale sarà l'alleanza con gli altri (diplomazia di tipo *Cessate il fuoco*);
    Lo scopo di questa modalità è quello di conquistare tutti gli insediamenti principali (quindi foro e centro del villaggio) del nemico: qualora questo accada, il giocatore sconfitto ha due minuti di tempo per rivalersi e riconquistare almeno una delle strutture perdute, a patto che abbia unità militari che può controllare.

    * **Multigiocatore**. Si tratta a tutti gli effetti di una partita in schermaglia, quindi con tutte le regole e personalizzazioni tipiche di quella categoria, ma potrà essere giocata esclusivamente da giocatori umani.

6. Una partita, qualsiasi sia la modalità, è giocata all'interno di una mappa, vale a dire un paesaggio realistico in cui sono presenti contemporaneamente la natura e la mano dell'uomo: si alterneranno, quindi, zone erbose, con parecchia vegetazione, o montane con la neve, a zone civilizzate in cui ci saranno solo edifici e sculture dedicate agli dèi;

7. Se non è impostata una flag che dica diversamente, durante ogni partita, a intervalli regolari di due ore, avviene il cambio di stagione: tale variazione avrà valore puramente estetico e non influenzerà in alcun modo le dinamiche del gioco (cambieranno perciò le texture e il colore della luminosità solare/ambientale). Durante la primavera e l'estate, la probabilità di precipitazioni è pressoché nulla, mentre in autunno è molto alta; in inverno, invece, la possibilità è la stessa dell'autunno, ma la pioggia sarà rimpiazzata dalla neve. Con un'altra flag è possibile stabilire anche l'attivazione del ciclo giorno/notte, che avrà lo stesso funzionamento del cambio di stagioni (quindi valore puramente estetico, ma anche strategico per i giocatori umani).

8. Premendo il tasto BARRA SPAZIATRICE è possibile aprire la minimappa, ossia una rappresentazione in scala ridotta della mappa di gioco. In essa sono contenuti il terreno, gli edifici (contornate da un bordo del colore corrispondente a quello del giocatore che li possiede), le decorazioni, rappresentati in maniera fedele, mentre le unità sono rappresentate da puntini del colore corrispondente a quello del giocatore che le controlla; tuttavia, la minimappa mostra soltanto gli oggetti che si trovano nelle aree esplorate e, se è attiva anche la nebbia di guerra, le unità che non si trovano in una zona coperta dalla nebbia.

9. Nel corso del gioco alcune parti della mappa appaiono coperte da una bruma semitrasparente, mentre altre si trovano nella più assoluta oscurità. Questo effetto si chiama *nebbia di guerra*. Le zone oscure sono quelle che non sono state ancora esplorate e su cui, naturalmente, non si hanno informazioni. Quando passi in queste aree con le tue unità, il terreno resta allo scoperto e puoi vedere le unità e le strutture del nemico che si trovano lì.

10. Tra gli oggetti interagibili possiamo annoverare *le unità* e *gli edifici*. Mentre è possibile reclutare unità, quindi in sostanza crearne di nuove, non è ugualmente possibile costruire edifici quando la partita è già stata avviata. Analizzando la struttura di questi due concetti, partendo dall'ultimo, possiamo identificare gli edifici come l'insieme di tutte quelle strutture che il giocatore può controllare e che hanno il compito di immagazzinare risorse, addestrare soldati, svolgere ricerche tecnologiche, chiedere favori agli dèi o celebrare alcuni potenti rituali e, in alcuni casi, nascondere e proteggere le unità da eventuali attacchi nemici.

11. Ogni elemento interagibile in gioco, sia questo una unità o un edificio, appartiene a una civiltà: questa servirà, oltre che a fare una distinzione grafica, a suddividere i popoli per caratteristiche, cultura, tecnologie e punti di forza e debolezza.

12. Le strutture si dividono in dieci categorie fondamentali:
    * **Elementi della fortezza** : comprendono, in via generica (quindi i nomi variano a seconda della civiltà di appartenenza, ma il modo in cui funziona resta quello), *il Foro* – che è l'edificio centrale da cui dipende tutto l'insediamento –, *la Caserma* – che è l'edificio adibito al reclutamento delle nuove unità a patto che si possiedano le risorse e gli equipaggiamenti necessari –, *la Fucina* – che è l'edificio preposto alla produzione degli armamenti necessari a reclutare unità avanzate –, *la Taverna* – che è l'edificio in cui si scambiano risorse per ottenere dei vantaggi strategici –, *il Tempio* – che è l'edificio in cui si ottengono i servigi dei sacerdoti e dei poteri degli dèi –, *l'Arena* – che è l'edificio in cui si reclutano gli eroi e si migliorano le caratteristiche delle unità – e *la Muraglia* – che comprende le mura, in cui vengono generate continuamente ma a ritmo decrescente *le sentinelle*, e i cancelli che servono a proteggere la fortezza dagli assalti dei nemici, impedendone il transito;
    * **Avamposti e forti** : che comprendono tutti gli edifici in cui possono essere ottenute o scambiate le risorse e in cui possono rifugiarsi gli eserciti, ottenendo l'immunità agli attacchi delle truppe nemiche, o appostandosi per realizzare delle imboscate;
    * **Porti** : sono i luoghi in cui vengono prodotte le imbarcazioni per il trasporto di risorse da un luogo all'altro e quelle da guerra, per il trasporto di truppe e il combattimento marittimo;
    * **Villaggi** : sono i centri adibiti alla produzione dei viveri. Essi sono composti dalle case, che aumentano il limite massimo di abitanti residenti di dieci unità, e il centro del villaggio da cui dipende tutto l'insediamento;
    * **Accampamenti teutonici** : sono edifici particolari protetti da diversi cavalieri, che attaccano tutto e tutti e che impediscono che la tenda venga conquistata; tuttavia, il procedimento di conquista della struttura non è lo stesso degli altri insediamenti ed è, infatti, necessario che siano stati uccisi tutti i difensori prima di ottenerne il controllo, poiché l'edificio non ha lealtà. Quando questo accade, infatti, la struttura è in grado di apportare un discreto supporto al nuovo proprietario giacché, in cambio di alcune unità di cibo, permette di reclutare ulteriori cavalieri difensori sotto il proprio controllo;
    * **Locande** : sono delle peculiari strutture che si trovano esclusivamente nella modalità Avventura a più mappe, e che consentono ad alcune unità prescelte, che dovranno avere attivo l'attributo *Gruppo*, di trasferirsi da una mappa a un'altra, mantenendo inalterate le proprie statistiche, simulando un viaggio: per raggiungere una Locanda collocata in un'altra mappa, è essenziale che le unità l'abbiano visitata almeno una volta;
    * **Altari dei sacrifici** : sono dei luoghi in cui si radunano i sacerdoti, le maghe, gli sciamani o i druidi per celebrare, in cambio di un certo valore di lealtà, alcuni potenti rituali che avranno influenza su tutte le unità e le strutture di qualsiasi giocatore nella mappa, compreso anche colui che celebra il rituale. Il controllo dell'altare, tuttavia, spetta al giocatore che porta il maggior numero di unità spirituali al sacrificio. L'elenco dei rituali attuabili è il seguente:
        * **Vento della saggezza.** Aumenta di 1 il livello di tutte le unità esistenti, fino a un valore massimo di 20 (punti lealtà necessari: 10);
        * **Carestia**. Riduce, durante i prossimi due minuti, i viveri di ogni fortezza, forte, villaggio e unità di rifornimento (punti lealtà necessari: 20);
        * **Rituale della vitalità**. Restituisce tutta l'energia alle unità, ogni 5 secondi. L'effetto dura 30 secondi (punti lealtà necessari: 30);
        * **Pioggia d'oro**. Per 2 minuti aumenta lentamente le scorte d'oro di tutte le strutture (punti lealtà necessari: 40);
        * **Castigo spirituale**. Riduce, per un minuto, la salute attuale di tutte le unità spirituali (punti lealtà necessari: 50);
        * **Sete di sangue**. Per un minuto tutti i guerrieri alleati infliggono danni per un ammontare indipendente dal valore di difesa del nemico (punti lealtà necessari: 60);
        * **Rivolta teutonica**. Provoca una rivolta in tutti gli accampamenti teutonici (punti lealtà necessari: 70);
    * **Catapulte** : sono delle strutture particolari il cui compito è quello di distruggere qualsiasi edificio, con grandi proiettili come palle di fuoco, lance infuocate enormi e così via. A differenza delle altre costruzioni, esse sono in grado di interagire con l'ambiente circostante e possono essere costruite o smantellate dalle truppe militari (massimo 10 per catapulta), in qualsiasi punto calpestabile della mappa: maggiore è il numero di soldati all'interno della catapulta, maggiore sarà il rateo di fuoco.
    * **Grotte** : sono passaggi sotterranei che uniscono due zone distanti tra loro. Le unità e gli eserciti le utilizzano per spostarsi rapidamente da un luogo all'altro senza essere individuati, per potere così cogliere di sorpresa il nemico. La peculiarità di questa struttura è che deve essere sistemata nel mondo di gioco sempre in coppia, garanzia questa che siano presenti sempre sia l'entrata sia l'uscita.
    * **Luoghi con oggetti magici** : sono particolari costruzioni più o meno rupestri, fatte dall'uomo in favore degli dèi o per scacciare i malefici, nelle quali, a intervalli regolari, compariranno degli oggetti in grado di fornire poteri straordinari al possessore.

13. Tutti gli insediamenti hanno i seguenti parametri:
    * **Salute attuale** : indica i punti vita attualmente posseduti dall'edificio;
    * **Salute massima** : indica i punti vita massimi posseduti dall'edificio;
    * **Oro** : indica la quantità d'oro attualmente immagazzinata nell'edificio. Può essere autoprodotta nelle fortezze o nei forti dell'oro con più di 2.ooo pezzi della valuta;
    * **Viveri** : indica la quantità di cibo attualmente immagazzinata nell'edificio. Può essere autoprodotta nelle fortezze o nei villaggi;
    * **Popolazione attuale** : indica il numero di abitanti attualmente residenti in quell'insediamento. Questo valore comparirà solo nei villaggi e nelle fortezze;
    * **Popolazione massima** : indica il numero massimo di abitanti che possono risiedere in quell'insediamento;
    * **Lealtà** : mostra il livello di fedeltà che quell'edificio o insediamento ha nei confronti del giocatore che lo possiede. Tale valore parte da 0 fino a 100. Mentre può accrescere sia automaticamente (quindi partirà dal minimo finché non avrà raggiunto il massimo, aumentando di uno in uno), sia manualmente (attraverso la presenza di alleati nelle vicinanze, che ne accrescono il valore di uno ogni due secondi per ogni unità e non può comunque superare il dieci) non diminuisce mai, se non quando le unità nemiche provano a conquistarlo. Quest'ultima regola non si applica all'altare dei sacrifici, in cui, per aumentare la lealtà è necessaria la presenza delle unità spirituali nelle immediate vicinanze, mentre per diminuirla si devono avviare i relativi rituali;

14. Le unità si dirigeranno verso la struttura principale da cui dipende un insediamento di un giocatore nemico quando questo viene selezionato per la cattura, e le conseguenze che ne scaturiscono dipendo fondamentalmente dal tipo di struttura che prendiamo in esame.
    * **Fortezze** : il giocatore prenderà il controllo del foro e di tutte le strutture a esso collegate, fornendo le risorse e il livello tecnologico acquisito da quell'insediamento anche se non gli era appartenuto fino a quel momento;
    * **Villaggi** : il giocatore prenderà il controllo del centro del villaggio e di tutte le case a esso collegate, fornendo i viveri di quell'insediamento anche se non gli era appartenuto fino a quel momento;
    * **Avamposti, forti e porti** : essendo essi singoli elementi, saranno conquistati distintamente l'uno dall'altro e forniranno, al giocatore che ne prenderà il controllo, tutte le risorse che custodiscono al loro interno;

15. Un edificio può essere attaccato e danneggiato soltanto dalle unità a distanza, che siano esse a piedi, a cavallo o navi, o dalle catapulte e macchine d'assedio: se all'interno si sono rifugiate alcune unità, queste subiranno dei danni per ogni colpo che la struttura riceve, finché non saranno morte. Se l'edificio preso in esame è, invece, una casa o un centro del villaggio, questo inizierà una perdita inesorabile della popolazione, in proporzione ai danni subiti. Quando la salute scende sotto il 10% della salute massima, l'edificio diventa inagibile e può ritornare al suo pieno funzionamento soltanto dopo averlo riparato tramite l'apposito comando e dietro il pagamento di una somma d'oro.

16. Le unità rappresentano l'elemento con cui il giocatore potrà esercitare la propria egemonia e la propria influenza su tutto il territorio. Possiamo suddividerli in 10 categorie:
    * **Unità di fanteria** : appartengono a questa categoria tutti i soldati che combattono corpo a corpo e che non usano mezzi per spostarsi o combattere (ad esempio un legionario, un pretoriano o un miliziano);
    * **Unità da lancio** : appartengono a questa categoria tutti i soldati che combattono dalla distanza con armi più o meno convenzionali (archi e frecce, ma anche fionde e lance o giavellotti) e che non usano mezzi per spostarsi;
    * **Unità a cavallo** : appartengono a questa categoria tutti quei soldati che combattono sia corpo a corpo sia con armi a distanza e che utilizzano un cavallo o un carro trainato da animali per spostarsi e combattere;
    * **Unità speciali** : appartengono a questa categoria tutti quei soldati che possono essere reclutati a condizioni particolari e in numero limitato, data la loro forza e le caratteristiche peculiari (ad esempio elefanti da guerra cartaginesi o guerriglieri iberici);
    * **Eroi** : appartengono a questa categoria alcuni soldati speciali, che sono in sella a un cavallo e che, date le loro attitudini in combattimento, sono in grado di utilizzare abilità speciali che forniscono supporto in uno scontro, e di guidare una moltitudine di soldati in battaglia;
    * **Imbarcazioni** : appartengono a questa categoria tutte quelle navi e barche atte al trasporto di unità da una costa a un'altra e al combattimento con frecce infuocate;
    * **Sistemi per il trasporto delle risorse** : appartengono a questa categoria tutti quei mezzi con cui è possibile spostare oro o cibo da un insediamento a un altro. Tale spostamento può avvenire per via terrena, con muli e carri, o per via marittima;
    * **Macchine d'assedio** : appartengono a questa categoria tutti quei marchingegni costruiti con l'ausilio di soldati, come le arieti, il cui compito è limitato esclusivamente allo sfondamento dei cancelli delle fortezze avversarie e al danneggiamento dei forti.
    * **Sentinelle** : fa parte di questa categoria un caratteristico tipo di arcieri, non controllabili dal giocatore, il cui aspetto grafico replica fedelmente quello della rispettiva civiltà cui appartengono, che pattugliano le mura della fortezza e che hanno il compito di difenderla dagli attacchi nemici.
    * **Altre** : appartengono a questa categoria residua tutte quelle unità che non possono essere catalogate in quelle precedenti, come ad esempio gli animali (cervi, corvi, aquile, lupi, cinghiali, leoni, pesci, cavalli e orsi) o gli spiriti evanescenti;

17. Ogni unità in gioco possiede diverse caratteristiche che la contraddistinguono.
    * **Salute attuale** : indica i punti vita attualmente posseduti dal guerriero;
    * **Salute massima** : indica i punti vita massimi posseduti dal guerriero;
    * **Livello** : indica il valore di combattimento del guerriero. Più è alto, maggiore sarà il danno inflitto e minore sarà il danno ricevuto. Il tetto massimo raggiungibile è fissato a *mille*. Esso influenza, inoltre, il colore delle parti metalliche delle armature;
    * **Esperienza attuale** : indica il valore di esperienza che l'unità possiede. Per poterlo accrescere è necessario, innanzi tutto, non aver raggiunto il livello massimo e poi sconfiggere unità nemiche in combattimento. L'aumento è determinato in proporzione alla differenza di livello tra l'attaccante e il bersaglio;
    * **Esperienza massima** : indica il valore massimo di esperienza. Una volta raggiunto, l'unità raggiunge il livello successivo;
    * **Tipo d'attacco** : indica la tipologia di arma utilizzata dal guerriero in combattimento. Può essere *tagliente* (spade, asce, mazze, martelli o zanne d'elefante) o *perforante* (lance, giavellotti, frecce, fionde o tridenti);
    * **Attacco minimo** : indica il valore minimo di danno che l'unità può infliggere al bersaglio e al di sotto del quale non può mai scendere;
    * **Attacco massimo** : indica il valore massimo di danno che l'unità può infliggere al bersaglio. Tale numero, però, può aumentare esponenzialmente in base al livello (l'aumento non comparirà nella barra in cima, ma sarà esclusivo della formula per il calcolo dei danni);
    * **Difesa contro le armi taglienti** : indica il valore di danno contro le armi taglienti che l'unità può assorbire;
    * **Difesa contro le armi perforanti** : indica il valore di danno contro le armi perforanti che l'unità può assorbire;
    * **Rifornimenti** : indica la quantità di viveri che l'unità possiede attualmente affinché gli venga garantita la sopravvivenza. Quando il valore raggiunge lo zero, il guerriero perde lentamente la salute fino al momento in cui non raggiunge il 10% della salute massima. Per ripristinarlo, invece, è necessario che il soldato si trovi in prossimità di una qualsiasi fonte di rifornimento, come villaggi, fortezze o unità per il trasporto di cibo. Nel momento in cui il valore è superiore a zero, l'unità recupererà lentamente la propria salute, finché non ha raggiunto il massimo;
    * **Rifornimenti massimi** : indica la quantità massima di viveri che l'unità può portare con sé;
    * **Velocità di movimento** : indica quanti metri al secondo l'unità percorre mentre cammina;
    * **Raggio d'azione** : indica la distanza minima richiesta (in metri) affinché l'unità possa attaccare;
    * **Campo visivo** : indica il diametro della circonferenza di cui ogni unità è dotata per rivelare le aree coperte dalla nebbia di guerra;
    * **Inventario** : indica il numero massimo di oggetti che l'unità può portare con sé. Il tetto massimo è fissato a *quattro* oggetti;
    * **Stamina** : indica il valore massimo di energia che l'unità possiede allo scopo di attivare le proprie abilità speciali;
    * **Abilità speciali** : sono tutte quelle skill passive (la cui attivazione, quindi, è indipendente dalla volontà del giocatore) di cui l'unità è dotata e che gli consentono di ottenere dei benefici in combattimento, o di assistere gli alleati. Tali abilità non si applicano agli eroi, che invece ne utilizzano altre. Nell'elenco successivo, si elencano tutte, col nome e il relativo effetto.
        * **Contenimento** : l'unità non può né infliggere danno né riceverlo se ha energia disponibile. Ogni attacco fallito consuma un punto di energia, mentre ogni colpo inflitto ne fornisce uno;
        * **Sfinimento** : ogni colpo riuscito su un avversario riduce l'energia del nemico di 2 punti (consuma *1 punto* energia);
        * **Ferocia** : l'unità recupera la quantità massima di energia quando sconfigge un nemico;
        * **Triplo colpo** : triplica il danno inflitto al nemico (consuma *1 punto* energia);
        * **Immunità** : permette all'unità di risultare indenne dal colpo inflitto dal nemico (consuma *1 punto* energia);
        * **Vitalità** : il guerriero recupera due punti energia anziché uno.
        * **Assorbimento** : l'unità riceve un numero di punti di salute proporzionale al danno causato al nemico (consuma *1 punto* energia);
        * **Rianimazione:** il guerriero recupera un punto di energia per ogni colpo inflitto al nemico;
        * **Resistenza** : il guerriero riceve 5 punti di salute massima in più per ogni livello raggiunto;
        * **Attacco esperto** : somma il livello dell'unità al suo valore di attacco (consuma *1 punto* energia);
        * **Perforazione** : il soldato infligge il massimo danno senza considerare il valore di difesa del nemico (consuma *1 punto* energia);
        * **Danno riflesso** : restituisce all'attaccante tutto il danno ricevuto nella lotta corpo a corpo (consuma *1 punto* energia);
        * **Ostinazione** : infligge punti danno addizionali equivalenti al 10% della salute massima del nemico (consuma *1 punto* energia);
        * **Attacco speciale** : aggiunge al valore di attacco dell'unità il doppio dei punti di energia che possiede attualmente. Si attiva quando l'unità infligge con successo un colpo (consuma *1 punto* energia);
        * **Difesa speciale** : aggiunge al valore di attacco dell'unità il doppio dei punti di energia che possiede attualmente. Si attiva quando l'unità viene colpita (consuma *1 punto* energia);
        * **Colpo mortale** : l'unità elimina il nemico se il livello di salute di quest'ultimo dopo aver ricevuto il colpo è al di sotto del 50%. Questa caratteristica speciale non ha effetto sugli eroi (consuma *4 punti* energia);
        * **Attacco letale** : se l'unità possiede più del doppio dei punti di salute del nemico, e quest'ultimo riceve un colpo, il nemico verrà eliminato. Non si può usare contro gli eroi (consuma *4 punti* energia);
        * **Colpo esperto** : elimina con un solo colpo l'avversario se questo è molto inferiore a lui. Non si può usare contro gli eroi (consuma *4 punti* energia);
        * **Tattiche difensive** : somma il livello dell'unità al suo valore di difesa (consuma *1 punto* energia);
        * **Determinazione** : aumenta di 1 l'energia ogni volta che il guerriero viene colpito;
        * **Carica** : è una caratteristica delle unità a cavallo, che aumenta di 8 volte l'output di danno se il cavaliere non ha effettuato un attacco per più di dieci secondi (consuma *4 punti* energia);
        * **Rigenerazione** : l'unità recupera la salute più rapidamente (consuma *4 punti* energia);
        * **Foga** : aumenta la capacità di attacco dell'unità che ne dispone, a danno della sua salute. La salute che perde si trasforma in danno addizionale per il nemico. La perdita di salute dell'unità che dispone della "Foga" è limitata: non può mai scendere al di sotto del 50% della sua salute massima (consuma *4 punti* energia);
        * **Vendetta** : quando l'unità viene sconfitta, 100 punti di danno al suo assassino;
        * **Trionfo** : l'unità ripristina interamente la propria salute quando sconfigge un nemico;
        * **Attacco velenoso** : il guerriero infetta il nemico e lo fa ammalare, sottraendogli salute costantemente, senza scendere mai sotto il 10% della sua salute massima. L'effetto del malanno scompare quando l'unità recupera tutta la salute (consuma *2 punti* energia);
        * **Mimetismo** : l'unità che rimane ferma per 10 secondi diventa invisibile per i nemici;
        * **Occultamento** : permette all'unità di rendersi o rendere invisibile un'altra unità amica (consuma *1 punto* energia);
        * **Istruzione** : il guerriero riceve un punto di esperienza per ogni colpo inflitto a un nemico con più esperienza (consuma *1 punto* energia);
        * **Iniziazione** : l'unità che possiede questa caratteristica regala esperienza alle unità alleate con meno esperienza. (Consuma *1 punto* energia)
        * **Libertà** : l'unità che possiede questa caratteristica non può essere assegnata a un eroe.
        * **Esaurimento vitale** : riduce a zero i punti di energia del bersaglio. (Consuma *1 punto* energia)
        * **Maledizione** : permette all'unità di dirigere un maleficio al nemico affinché questo manchi il prossimo attacco. (Consuma *1 punto* energia)
        * **Guarigione** : cura l'unità bersaglio alleata, fornendole 20 punti salute più un punto salute per ogni livello dell'unità che possiede questa caratteristica speciale. (Consuma *1 punto* energia)
        * **Vista di lince** : rivela la presenza delle unità nemiche nascoste nel campo visivo.
        * **Danno espansivo** : L'unità che lo possiede infligge danno a tutti i nemici che si trovano nelle vicinanze sul campo di battaglia.

18. Gli eroi ereditano tutte le proprietà delle unità (salute, attacco, difesa e così via), ma possono comandare truppe di grado inferiore (minimo 50, fornendole dei particolari bonus e impartendo loro lo stesso comando simultaneamente) e utilizzare alcune skill, attive e passive, la cui efficacia incrementa quando le vengono assegnate dei punti (uno per ogni livello fino a un massimo di 10 per ciascuna). L'elenco successivo ne rappresenta la specificazione.
    * **Prestigio** *(passiva)*: per ogni punto assegnato a questa caratteristica, l'eroe potrà aumentare di 2 unità il numero di unità da lui comandate.
    * **Superiorità offensiva** *(passiva)*: per ogni punto assegnato incrementa di uno il valore di attacco (minimo e massimo) dei guerrieri comandati all'eroe.
    * **Superiorità difensiva** *(passiva)*: ogni punto aumenta di uno il valore di difesa (tagliente e perforante) dei guerrieri assegnati all'eroe.
    * **Marcia forzata** *(passiva)*: per ogni punto assegnato a questa caratteristica, aumenta la velocità di movimento delle unità vincolate all'eroe del 5%.
    * **Resistenza eroica** *(passiva)*: ogni punto assegnato comporta un aumento di 100 punti nel valore di salute massima dell'eroe.
    * **Erudizione** *(passiva)*: le unità assegnate all'eroe aumentano di 2 livelli per ogni punto assegnato a questa caratteristica speciale (fino a un massimo di 12).
    * **Maestria** *(passiva)*: per ogni punto assegnato a questa caratteristica aumenta del 2% l'esperienza che l'eroe trasferisce ai guerrieri che sono al suo seguito.
    * **Attacco eroico** *(passiva)*: aumenta il valore d'attacco (massimo) dell'eroe di 5 punti per ogni punto assegnato a questa caratteristica.
    * **Movimento sfuggente** *(passiva)*: la probabilità dei guerrieri di schivare un attacco nemico aumenta del 2% per ogni punto assegnato a questa caratteristica.
    * **Recupero** *(passiva)*: i guerrieri avranno, per ogni punto assegnato, il 10% di probabilità di ricevere 5 punti extra di salute quando si stanno alimentando.
    * **Esplorazione** *(passiva)*: per ogni punto assegnato a questa caratteristica aumenta progressivamente il campo visivo dell'eroe.
    * **Euforia** *(passiva)*: quando sconfiggono un nemico, le unità assegnate all'eroe avranno il 10% di possibilità per ogni punto assegnato a questa caratteristica di ottenere 3 punti di energia.
    * **Vigore** *(passiva)*: per ogni punto assegnato aumenta del 10% la possibilità dei guerrieri comandati dall'eroe di ottenere un punto di energia addizionale mentre si alimentano.
    * **Fedeltà** *(passiva)*: ogni cinque secondi l'eroe sottrae a uno dei suoi guerrieri, scelto a caso, 20 punti di salute per ogni punto assegnato a questa caratteristica se ha un ammontare di punti salute inferiori al massimo.
    * **Saggezza** *(passiva)*: quando uno dei suoi guerrieri muore, l'eroe ha un 5% di probabilità per ogni punto assegnato a questa caratteristica di accumulare 1 punto di esperienza.
    * **Chiamata alla difesa** *(attiva – 4 punti energia)*: quando si utilizza aumenta di 20 punti il la difesa delle unità per un secondo, e ne estende l'effetto di 2 secondi per ogni punto assegnato a questa caratteristica.
    * **Gesta** *(attiva – 4 punti energia)*: quando si utilizza, questa caratteristica permette ai guerrieri di ignorare il valore di difesa e le skill difensive dei nemici per un secondo, e ne estende l'effetto di tanti secondi quanti sono i punti assegnati.
    * **Prodezza** *(attiva – 4 punti energia)*: Quando si utilizza, per 5 secondi le unità vincolate all'eroe aumentano di tanti livelli quanti sono i punti assegnati a questa caratteristica.
    * **Sopravvivenza** *(passiva)*: ogni 5 secondi i soldati hanno, il 10% di possibilità per ogni punto assegnato di recuperare la salute senza necessità di alimentarsi.
    * **Carisma** *(attiva – 6 punti energia)*: quando si utilizza aggiunge all'energia dei guerrieri 1 punto per ogni punto assegnato a questa caratteristica.
    * **Tregua** *(attiva – 6 punti energia)*: quando si utilizza, le unità non infliggeranno né riceveranno alcun danno per tanti secondi quanti sono i punti assegnati a questa caratteristica.
    * **Guarigione eroica** *(attiva – 4 punti energia)*: quando si utilizza, l'eroe recupera 100 punti salute.
    * **Audacia** *(attiva – 6 punti energia)*: riduce del 50% la salute e raddoppia il valore di attacco (minimo e massimo) delle unità vincolate all'eroe. Il suo effetto dura un secondo, più un secondo addizionale per ogni punto assegnato.
    * **Aggressione** *(attiva – 6 punti energia)*: Quando si utilizza, diminuisce di 5 punti la salute e aggiunge 1 punto all'energia delle unità per ogni punto assegnato a questa caratteristica.
    * **Guarigione** *(attiva – 6 punti energia)*: restituisce ai guerrieri comandati dall'eroe 10 punti salute per ogni punto assegnato a questa caratteristica.

19. Quando le unità vengono assegnate all'eroe, si disporranno automaticamente in formazione e continueranno a tenerla ogniqualvolta stanno ferme oppure camminano: il vantaggio della formazione è che essa fornisce bonus alle statistiche delle unità se queste stanno usando il comando "mantieni la posizione". Esistono quattro tipi di formazione, elencate ed esaminate di seguito, con i relativi bonus.
    * **In linea:** le unità formano una o più linee davanti all'eroe. (+2 attacco minimo e massimo armi taglienti, +2 difesa perforante e tagliente, +20% attacco minimo e massimo armi perforanti);
    * **A quadrato** : le unità si dispongono attorno all'eroe formando un anello protettivo. (+6 difesa perforante e tagliente);
    * **Cavalleria sui fianchi** : tutte le unità di cavalleria si dispongono sui fianchi delle restanti truppe, che invece assumono la formazione "*In linea*". (+4 attacco minimo e massimo armi taglienti, +20% attacco minimo e massimo armi perforanti);
    * **In blocco** : le unità formano un blocco compatto davanti all'eroe, simulando uno scudo quadrato. (+4 livelli);

20. Il combattimento tra le truppe, di terra e di mare, può avvenire corpo a corpo o a distanza. Mentre gli attacchi corpo a corpo hanno una possibilità di colpire il bersaglio del 100%, quelli a distanza hanno una possibilità del 50% (per simulare l'imprecisione degli arcieri);

21. Durante una partita, qualsiasi sia la modalità di gioco, è possibile ordinare alle unità di raccogliere un oggetto magico, che viene generato ad intervalli regolari in particolari strutture, chiamate **luoghi con oggetti magici**. Tali oggetti si distinguono l'uno dall'altro per il potere che conferiscono e per il modo con cui questo potere può essere usato: a questo proposito, rappresentiamo l'elenco completo.
    * **Denti d'orso** : aumenta di 4 punti il danno massimo;
    * **Amuleto di erbe** : aumenta di 4 punti la difesa perforante;
    * **Cintura di forza** : aumenta di 4 punti la difesa tagliente;
    * **Spighe di segale** : distribuiscono 200 unità di viveri tra le unità amiche più vicine;
    * **Pelle di serpente** : aumenta l'attacco minimo e massimo di 10 punti;
    * **Cintura di serpenti** : aumenta l'attacco minimo e massimo di 30 punti;
    * **Piuma d'aquila** : aumenta la salute massima di 200 punti;
    * **Amuleto di piume** : aumenta la salute massima di 400 punt+i;
    * **Cintura dei re** : aumenta di 600 punti la salute massima e di 10 punti la difesa tagliente e perforante;
    * **Funghi velenosi** : usandoli, aumentano permanentemente di un punto il livello del portatore, al costo del 90 % di salute;
    * **Erbe medicinali** : ripristina completamente la salute del possessore;
    * **Acqua curativa** : distribuisce 1.000 punti di salute tra tutte le unità amiche nelle vicinanze;
    * **Zanna di cinghiale** : aggiunge 25 punti di esperienza. Quando viene usato, danneggia l'avversario, sottraendo salute al possessore;
    * **Denti di cinghiale** : aumenta il livello del possessore di 5 punti;
    * **Corno della vittoria** : infligge 60 punti di danno a chi lo porta e a 12 unità nemiche vicine;
    * **Ceneri del druido** : curano il loro portatore e le 8 unità amiche più vicine;
    * **Dito della morte** : elimina 3 unità vicine. Non ha effetto sugli eroi;
    * **Guanti della salute** : aumentano la salute di 1.200 punti. Quando vengono usati curano un'unità amica sottraendo salute a chi li possiede
    * **Pietra di fuoco** : aumenta di 60 punti l'attacco massimo. Quando viene usata cura il suo portatore, sottraendo salute a un'unità amica;
    * **Pietra sanguinaria** : nessun effetto;
    * **Gemma spenta** : la gemma è ancora troppo debole per incanalare il Potere della Dea;
    * **Gemma splendente** : ora la gemma può incanalare il Potere della Dea;
    * **Gemma del potere** : la gemma fa scorrere il Potere della Dea nelle tue vene;
    * **Dono degli dèi** : guarisce completamente il possessore quando uccide un nemico, quindi scompare;
    * **Bottino di guerra** : aggiunge 100 pezzi d'oro all'insediamento in cui sta entrando l'unità che lo possiede, quindi scompare;
    * **Anello dei veterani** : quando l'unità che lo porta sconfigge un nemico, si trasforma nella medaglia dei veterani;
    * **Medaglia dei veterani** : aggiunge 6 punti all'attacco minimo e massimo;
