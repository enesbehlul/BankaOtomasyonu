#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void ekraniTemizle() { system("@cls||clear"); }

int SIZE = 15;
int MAX_BANKA_MUSTERI = 20;
int MAX_BANKA_HESAP = 50;
int MAX_BANKA_DEKONT = 100;
int MAX_MUSTERI_HESAP = 10;

int GUN, AY, YIL;

typedef enum musteriTipi
{
    Bireysel,
    Ticari
} musteriTipi;

typedef enum islemTuru
{
    ParaCekme,
    ParaYatirma,
    HavaleGonderme,
    HavaleAlma
} islemTuru;

typedef struct Hesap
{
    int id;
    int musteriId;
    int bakiye;
} Hesap;

//tarih eklenecek UNUTMA!
typedef struct Musteri
{
    // musteri no, id olarak tutuluyor
    int id;
    char ad[9];
    char soyad[15];
    char dogumYeri[15];
    musteriTipi tip;
    // bir musterinin en fazla 10 hesabi olabilir
    Hesap *hesaplar;
} Musteri;

typedef struct Dekont
{
    int ay, yil, gun;
    int hesapId;
    int gondericiId;
    int tutar;
    int aliciHesapId;
    int aliciId;
    islemTuru islemTuru;
} Dekont;

typedef struct Banka
{
    Musteri *musteriler;
    Hesap *hesaplar;
    Dekont *dekontlar;
} Banka;

typedef struct Tarih
{
    int ay;
    int gun;
    int yil;
} Tarih;

int tarih_karsilastir(struct Tarih t1, struct Tarih t2)
{
    if (t1.yil < t2.yil)
        return -1;

    else if (t1.yil > t2.yil)
        return 1;

    if (t1.yil == t2.yil)
    {
        if (t1.ay < t2.ay)
            return -1;
        else if (t1.ay > t2.ay)
            return 1;
        else if (t1.gun < t2.gun)
            return -1;
        else if (t1.gun > t2.gun)
            return 1;
        else
            return 0;
    }
}

void dosyayaHesapKaydet(Hesap *);
void musteriHesaplariGuncelle(Musteri *);

int rastgeleSayiOlustur()
{
    // her seferinde farkli bir random sayisi icin gereken kod
    srand(time(NULL));
    // 1 ile 100 arasi rastgele sayi
    return rand() % 100 + 1;
}

int get_tarih(int i)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    return (i == 2) ? tm.tm_year + 1900 : (i == 1) ? tm.tm_mon + 1 : (i == 0) ? tm.tm_mday : -1;
}

Hesap *createHesap(int id, int musteriId, int bakiye)
{
    Hesap *hesap = (Hesap *)malloc(sizeof(Hesap));
    hesap->id = id;
    hesap->musteriId = musteriId;
    hesap->bakiye = bakiye;
    return hesap;
}

Musteri *createMusteri(int id, char *ad, char *soyad, char *dogumYeri, int musteriTuru)
{
    Musteri *musteri = (Musteri *)malloc(sizeof(Musteri));
    musteri->hesaplar = (Hesap *)malloc(sizeof(Hesap) * MAX_MUSTERI_HESAP);
    musteri->id = id;
    strcpy(musteri->ad, ad);
    strcpy(musteri->soyad, soyad);
    strcpy(musteri->dogumYeri, dogumYeri);
    musteri->tip = musteriTuru;

    int i;
    Hesap hesap;
    hesap.id = -1;
    for (i = 0; i < MAX_MUSTERI_HESAP; i++)
    {
        musteri->hesaplar[i] = hesap;
    }

    return musteri;
}

Dekont *createDekont(int hesapId, int tutar, int aliciHesapId, islemTuru islem, int gondericiId, int aliciId, int gun, int ay, int yil)
{
    Dekont *dekont = (Dekont *)malloc(sizeof(Dekont));
    dekont->hesapId = hesapId;
    dekont->aliciHesapId = aliciHesapId;
    dekont->tutar = tutar;
    dekont->islemTuru = islem;
    dekont->gun = gun;
    dekont->ay = ay;
    dekont->yil = yil;
    dekont->aliciId = aliciId;
    dekont->gondericiId = gondericiId;
    return dekont;
}

// burada musteri bilgileri txt dosyadan okunacak
// daha once kullanildiysa musteri struct'i, kullanilmadiysa null
// degeri dondurulecek
Musteri *getMusteriById(int id)
{
    int musteriId = 0;
    islemTuru islemTuru;

    char *ad = malloc(sizeof(char) * SIZE);
    char *soyad = malloc(sizeof(char) * SIZE);
    char *dogumYeri = malloc(sizeof(char) * SIZE);
    char *musteriTuru = malloc(sizeof(char) * SIZE);

    // musteriler dosyasini okuma modunda aciyoruz 'r' okuma modu
    FILE *fileBireysel, *fileTicari;
    fileBireysel = fopen("bireyselMusteriler.txt", "r");

    Musteri *musteri;

    if (fileBireysel != NULL)
    {
        while (!feof(fileBireysel))
        {
            //dosyadan okunan veri virgul ile ayrilip degiskenlere ataniyor
            fscanf(fileBireysel, "%d,%[^,],%[^,],%s\n", &musteriId, ad, soyad, dogumYeri);
            //eger daha once kullanilmis bir musteri id ise
            if (id == musteriId)
            {
                //return isleminden once dosya kapatiliyor
                fclose(fileBireysel);

                // musteri bulundugu icin o bilgilerle yeni musteri olusturup donduruyoruz
                musteri = createMusteri(musteriId, ad, soyad, dogumYeri, 0);
                musteriHesaplariGuncelle(musteri);
                return musteri;
            }
            // ayni degiskenleri dosyadan okunacak diger satirlar icin de kullanmak icin sifirliyoruz.
            //buna gerek yok sanirim, yorum olarak birakiyorum
            /*
            strcpy(ad, "");
            strcpy(soyad, "");
            strcpy(dogumYeri, "");
            strcpy(musteriTuru, "");
            */
        }
        fclose(fileBireysel);
    }

    // ayni islemi ticari musteriler icin de yapiyoruz
    fileTicari = fopen("ticariMusteriler.txt", "r");
    if (fileTicari != NULL)
    {
        while (!feof(fileTicari))
        {
            fscanf(fileTicari, "%d,%[^,],%[^,],%s\n", &musteriId, ad, soyad, dogumYeri);

            if (id == musteriId)
            {
                //return isleminden once dosya kapatiliyor
                fclose(fileTicari);

                musteri = createMusteri(musteriId, ad, soyad, dogumYeri, 1);
                musteriHesaplariGuncelle(musteri);
                return musteri;
            }
        }
        fclose(fileTicari);
    }
    return NULL;
}

Hesap *getHesapByHesapId(int id)
{
    FILE *hesapFile;
    hesapFile = fopen("hesaplar.txt", "r");

    int hesapId;
    int musteriId;
    int bakiye;

    if (hesapFile != NULL)
    {
        while (!feof(hesapFile))
        {
            fscanf(hesapFile, "%d,%d,%d\n", &hesapId, &musteriId, &bakiye);

            if (id == hesapId)
            {
                //return isleminden once dosya kapatiliyor
                fclose(hesapFile);
                return createHesap(hesapId, musteriId, bakiye);
            }
        }
        fclose(hesapFile);
    }
    return NULL;
}

// bir musteriye ait tum hesaplari referans olarak diziye kaydetme
void musteriHesaplariGuncelle(Musteri *musteri)
{
    FILE *hesapFile;
    hesapFile = fopen("hesaplar.txt", "r");

    int hesapId;
    int musteriId;
    int bakiye;

    int counter = 0;

    if (hesapFile != NULL)
    {
        while (!feof(hesapFile))
        {
            fscanf(hesapFile, "%d,%d,%d\n", &hesapId, &musteriId, &bakiye);

            if (musteri->id == musteriId)
            {
                musteri->hesaplar[counter++] = *createHesap(hesapId, musteriId, bakiye);
            }
        }
        fclose(hesapFile);
    }
}

void bankaHesaplariGuncelle(Banka *banka)
{
    FILE *hesapFile;
    hesapFile = fopen("hesaplar.txt", "r");

    int hesapId;
    int musteriId;
    int bakiye;

    int counter = 0;

    if (hesapFile != NULL)
    {
        while (!feof(hesapFile))
        {
            fscanf(hesapFile, "%d,%d,%d\n", &hesapId, &musteriId, &bakiye);

            banka->hesaplar[counter++] = *createHesap(hesapId, musteriId, bakiye);
        }
        fclose(hesapFile);
    }
}

// program ilk baslatildiginda, banka struct'indaki
// diziler icin memory allocation yapmak icin
void bosBankaStructiOlustur(Banka *banka)
{
    banka->dekontlar = (Dekont *)malloc(sizeof(Dekont) * MAX_BANKA_DEKONT);
    banka->musteriler = (Musteri *)malloc(sizeof(Musteri) * MAX_BANKA_MUSTERI);
    banka->hesaplar = (Hesap *)malloc(sizeof(Hesap) * MAX_BANKA_HESAP);

    Dekont dekont;
    dekont.hesapId = -1;

    Musteri musteri;
    musteri.id = -1;

    Hesap hesap;
    hesap.id = -1;
    // 100 elemanlik dekont dizisi icin baslangicta bos olup olmadigi
    // kontrolu yapilabilmesi icin tum elemanlarini gonderici id'si -1
    // olan bir dekont structina esitliyoruz
    /* 
    Bunu yapmamizin sebebi, ornegin bankadaki tum hesaplari dosyaya yazacagiz diyelim,
    hesaplar icin 50 elemanlik bir dizi tanimladik ancak bankamizda 20
    adet hesap varsa, dizi uzerinde dolasirken 21. hesaba gelindiginde id'si
    -1 olacaktir ve bu durumda dongunun 50. elemana kadar devam etmesine gerek 
    yoktur diyebilmek icindir. Aksi durumda, 20'den 50 ye kadar dosyaya yazma 
    islemi devam eder. Var olmayan bilgileri dosyaya yazmis oluruz.
    */
    int i;
    for (i = 0; i < MAX_BANKA_DEKONT; i++)
    {
        banka->dekontlar[i] = dekont;
    }

    //ayni islemi hesaplar ve musteriler icin de yapiyoruz

    for (i = 0; i < MAX_BANKA_HESAP; i++)
    {
        banka->hesaplar[i] = hesap;
    }

    // her musteri kendi icinde hesaplar dizisi tutmaktadir dolayisiyla
    // o hesap dizisi de ayni isleme tabidir
    musteri.hesaplar = (Hesap *)malloc(sizeof(Hesap) * MAX_MUSTERI_HESAP);
    for (i = 0; i < MAX_MUSTERI_HESAP; i++)
    {
        musteri.hesaplar[i] = hesap;
    }

    for (i = 0; i < MAX_BANKA_MUSTERI; i++)
    {
        banka->musteriler[i] = musteri;
    }
}

// Daha onceden olusturulmus musteri ve hesap bilgilerini
// dosyalardan okuyup ilgili degiskenlere kaydediyoruz
// HESAP BILGILERI OKUMAYI UNUTMA!
// hesap bilgilerini banka icin okumayi UNUTMA!!
// Dekont bilgilerini okumayi UNUTMA!
void bankaBaslangicBilgileriDosyadanOku(Banka *banka)
{
    bosBankaStructiOlustur(banka);
    bankaHesaplariGuncelle(banka);

    int musteriId = 0;
    char *ad = malloc(sizeof(char) * SIZE);
    char *soyad = malloc(sizeof(char) * SIZE);
    char *dogumYeri = malloc(sizeof(char) * SIZE);
    char *musteriTuru = malloc(sizeof(char) * SIZE);

    FILE *fileBireysel = fopen("bireyselMusteriler.txt", "r");

    int counter = 0;

    if (fileBireysel != NULL)
    {
        while (!feof(fileBireysel))
        {
            fscanf(fileBireysel, "%d,%[^,],%[^,],%s\n", &musteriId, ad, soyad, dogumYeri);
            Musteri *musteri = createMusteri(musteriId, ad, soyad, dogumYeri, 0);
            musteriHesaplariGuncelle(musteri);
            banka->musteriler[counter++] = *musteri;
        }
        fclose(fileBireysel);
    }
    // ayni islemi ticari musteriler icin de yapiyoruz
    FILE *fileTicari = fopen("ticariMusteriler.txt", "r");
    if (fileTicari != NULL)
    {
        while (!feof(fileTicari))
        {
            fscanf(fileTicari, "%d,%[^,],%[^,],%s\n", &musteriId, ad, soyad, dogumYeri);
            Musteri *musteri = createMusteri(musteriId, ad, soyad, dogumYeri, 1);
            musteriHesaplariGuncelle(musteri);
            banka->musteriler[counter++] = *musteri;
        }
        fclose(fileTicari);
    }

    FILE *dekontFile = fopen("dekontlar.txt", "r");
    if (dekontFile != NULL)
    {
        int counter = 0;
        while (!feof(dekontFile))
        {
            fscanf(dekontFile, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                   &banka->dekontlar[counter].hesapId,
                   &banka->dekontlar[counter].aliciHesapId,
                   &banka->dekontlar[counter].tutar,
                   &banka->dekontlar[counter].islemTuru,
                   &banka->dekontlar[counter].gondericiId,
                   &banka->dekontlar[counter].aliciId,
                   &banka->dekontlar[counter].gun,
                   &banka->dekontlar[counter].ay,
                   &banka->dekontlar[counter].yil);
            counter++;
        }

        fclose(dekontFile);
    }
}

void dosyayaMusteriKaydet(Musteri *musteri)
{
    FILE *dosya;
    // musteri turune gore farkli dosyalara kayit
    if (musteri->tip == Bireysel)
    {
        // dosyayi yazma modunda aciyoruz, 'a' append modu append modundayken yazdigimiz
        // seyler dosyanin sonuna kaydediliyor, onceki bilgiler korunmus oluyor
        dosya = fopen("bireyselMusteriler.txt", "a");
    }
    else
    {
        dosya = fopen("ticariMusteriler.txt", "a");
    }

    fprintf(dosya, "%d,%s,%s,%s\n",
            musteri->id,
            musteri->ad,
            musteri->soyad,
            musteri->dogumYeri);

    fclose(dosya);
    puts("Musteri bilgileri dosyaya kaydedildi.");
}

void dosyayaHesapKaydet(Hesap *hesap)
{
    FILE *dosya;
    dosya = fopen("hesaplar.txt", "a");

    fprintf(dosya, "%d,%d,%d\n",
            hesap->id,
            hesap->musteriId,
            hesap->bakiye);

    fclose(dosya);
    puts("Hesap bilgileri dosyaya kaydedildi.");
}

void dosyayaDekontKaydet(Dekont *dekont)
{
    FILE *dosya;
    dosya = fopen("dekontlar.txt", "a");

    fprintf(dosya, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
            dekont->hesapId,
            dekont->aliciHesapId,
            dekont->tutar,
            dekont->islemTuru,
            dekont->gondericiId,
            dekont->aliciId,
            dekont->gun,
            dekont->ay,
            dekont->yil);

    fclose(dosya);
    puts("Dekont bilgileri dosyaya kaydedildi.");
}

void dekontOlusturVeDosyayaKaydet(Banka *banka, int hesapId, int tutar, int aliciHesapId, islemTuru islem, int gondericiId, int aliciId)
{
    Dekont *dekont = createDekont(hesapId, tutar, aliciHesapId, islem, gondericiId, aliciId, GUN, AY, YIL);
    int counter = 0;
    while (banka->dekontlar[counter].hesapId != -1 && counter < 100)
    {
        counter++;
    }
    if (banka->dekontlar[counter].hesapId == -1)
    {
        banka->dekontlar[counter] = *dekont;
    }

    dosyayaDekontKaydet(dekont);
}

// eger islem basariliysa 1 degilse 0 donduruyoruz
int bakiyeGuncelle(Hesap *hesap, int tutar)
{
    if (tutar == 0)
    {
        return 1;
    }

    if (tutar < 0 && hesap->bakiye < -tutar)
    {
        printf("Yetersiz bakiye.\n");
        return 0;
    }

    hesap->bakiye += tutar;
    printf("%d numarali hesabin bakiyesi guncellendi. Yeni bakiye: %d\n", hesap->id, hesap->bakiye);
    return 1;
}

void hesaplardakiDegisikligiDosyayaKaydet(Banka *banka)
{
    FILE *dosya;
    dosya = fopen("hesaplar.txt", "w");

    int counter = 0;
    while (banka->hesaplar[counter].id != -1)
    {
        //id nin -2 olmasi silinmis oldugunu gosterir
        if (banka->hesaplar[counter].bakiye != -2)
        {
            fprintf(dosya, "%d,%d,%d\n",
                    banka->hesaplar[counter].id,
                    banka->hesaplar[counter].musteriId,
                    banka->hesaplar[counter].bakiye);
        }

        counter++;
    }
    fclose(dosya);
    //puts("Hesap bilgileri dosyaya kaydedildi.");
}

// degisikligi bankaya yansitma sebebimiz, hesaplar.txt dosyasini bankanin hesaplar
// dizisi uzerinden guncelleyecek olmamizdan
void musteriHesabindakiDegisikligiBankayaYansit(Banka *banka, Hesap *hesap)
{
    int counter = 0;
    while (banka->hesaplar[counter].id != hesap->id && counter < 100)
    {
        counter++;
    }
    if (banka->hesaplar[counter].id == hesap->id)
    {
        banka->hesaplar[counter].bakiye = hesap->bakiye;
        //puts("Degisiklik bankaya yansidi.");
    }
}

// HEM BANKA'NIN DIZISINE HEM DOSYAYA EKLEMELISIN UNUTMA!!!
void musteriOlustur(Banka *banka)
{
    
    int id;
    // musteri bilgileri icin memoryden yer ayiriyoruz
    char *ad = malloc(sizeof(char) * SIZE);
    char *soyad = malloc(sizeof(char) * SIZE);
    char *dogumYeri = malloc(sizeof(char) * SIZE);
    int musteriTuru;
    char *devamMi = malloc(2 * sizeof(char));
    strcpy(devamMi, "e");
    while (!strcmp(devamMi, "e"))
    {
        ekraniTemizle();
        id = rastgeleSayiOlustur();
        // rastgele olusturulmus bir sayi daha once kullanilmis olabilir onu engellemek
        // icin kullanilmamis bir rastgele sayi gelene kadar dongu
        while (getMusteriById(id) != NULL)
        {
            id = rastgeleSayiOlustur();
        }
        puts("Bilgileri giriniz...");
        printf("\nMusteri>\n\tAd: ");
        scanf("%s", ad);
        printf("\n\tSoyad: ");
        scanf("%s", soyad);
        printf("\n\tDogum Yeri: ");
        scanf("%s", dogumYeri);
        puts("Bireysel musteri icin 0, ticari musteri icin 1");
        printf("\n\tMusteri turu: ");
        scanf("%d", &musteriTuru);
        // musteri turu icin 1 ya da sifir girilene kadar dongu
        while (musteriTuru != 1 && musteriTuru != 0)
        {
            puts("Lutfen dogru giris yapiniz.");
            puts("Bireysel musteri icin 0, ticari musteri icin 1");
            printf("\n\tMusteri turu: ");
            scanf("%d", &musteriTuru);
        }
        // musteri dosyaya kaydediliyor
        Musteri *musteri = createMusteri(id, ad, soyad, dogumYeri, musteriTuru);
        printf("Musteri olusturuldu. Id numarasi: %d\n", musteri->id);
        dosyayaMusteriKaydet(musteri);
        // musteriyi banka structinin musteriler dizisine de eklemeliyiz
        int counter = 0;
        while (banka->musteriler[counter].id != -1)
        {
            counter++;
        }
        banka->musteriler[counter] = *musteri;

        printf("\n\tDevam etmek istiyor musunuz e/h: ");
        scanf("%s", devamMi);
    }
}

// musteri referans olarak geciliyor ve olusturulan hesap
// musterinin hesaplar dizisine ekleniyor
// olusturulan hesabi bankanin hesaplar dizine eklemeyi UNUTMA
void hesapOlustur(Banka *banka, Musteri *musteri)
{
    ekraniTemizle();

    int id, bakiye;
    puts("Bilgileri giriniz...");
    printf("\nHesap>\n\tHesap numarasi: ");
    scanf("%d", &id);

    while (getHesapByHesapId(id) != NULL)
    {
        printf("\nBu numara daha once kullanilmis\n\tHesap numarasi: ");
        scanf("%d", &id);
    }

    printf("\n\tBaslangic bakiyesi: ");
    scanf("%d", &bakiye);

    Hesap *hesap = (Hesap *)malloc(sizeof(Hesap));
    hesap = createHesap(id, musteri->id, bakiye);

    // musteri hesaplarinda dolasiliyor ve bos indise hesap ekleniyor
    int i;
    for (i = 0; i < MAX_MUSTERI_HESAP; i++)
    {
        // COOK ZOR AAAAAABIIIIII
        if (musteri->hesaplar[i].id == -1)
        {
            printf("Hesabiniz olusturuldu. Toplam hesap sayiniz: %d\n", i + 1);
            musteri->hesaplar[i] = *hesap;
            break;
        }
    }
    // olusturulan hesap dosyaya kaydediliyor.
    dosyayaHesapKaydet(hesap);

    // olusturulan hesabi bankanin dizisine de ekliyoruz
    for (i = 0; i < MAX_BANKA_HESAP; i++)
    {
        if (banka->hesaplar[i].id == -1)
        {
            printf("Bu hesap bankamizin %d. hesabi.\n\n", i + 1);
            banka->hesaplar[i] = *hesap;
            break;
        }
    }
}

int musteriIslemLimitiHesapla(Musteri *musteri)
{
    int toplamCekilenMiktar = 0;

    int ay, yil, gun;
    int hesapId;
    int gondericiId;
    int tutar;
    int aliciHesapId;
    int aliciId;
    int islemTuru;

    FILE *dekontDosyasi = fopen("dekontlar.txt", "r");

    if (dekontDosyasi != NULL)
    {
        while (!feof(dekontDosyasi))
        {
            fscanf(dekontDosyasi, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                   &hesapId,
                   &aliciHesapId,
                   &tutar,
                   &islemTuru,
                   &gondericiId,
                   &aliciId,
                   &gun,
                   &ay,
                   &yil);

            if (gun == GUN && ay == AY && yil == YIL && islemTuru == 0 && gondericiId == musteri->id)
            {
                toplamCekilenMiktar += tutar;
            }
        }
    }
    return (musteri->tip == 0) ? 750 - toplamCekilenMiktar : 1500 - toplamCekilenMiktar;
}

// dekont eklemeyi unutma
void paraCek(Banka *banka, Musteri *musteri)
{
    ekraniTemizle();
    int limit = musteriIslemLimitiHesapla(musteri);
    if (limit <= 0)
    {
        puts("Islem limitiniz dolu.");
        return;
    }

    int hesapId;
    int tutar;
    printf("\n\tIslem yapmak istediginiz hesabinizin id'sini giriniz: ");
    scanf("%d", &hesapId);
    int counter = 0;
    // girilen hesap id musteriye ait mi?
    while (musteri->hesaplar[counter].id != hesapId && counter < 10)
    {
        counter++;
    }
    if (musteri->hesaplar[counter].id == hesapId)
    {
        printf("\n\tMiktar: ");
        scanf("%d", &tutar);
        if (tutar > limit)
        {
            printf("Kalan limitiniz: %d. Lutfen daha dusuk bir tutar giriniz.\n", limit);
            return;
        }

        int islem = bakiyeGuncelle(musteri->hesaplar + counter, -tutar);
        // islem basarisizsa, diger hesaplari kontrol et ve sor
        if (!islem)
        {
            int sayac = 0, toplamBakiye = 0, toplamCekilen = 0;
            int onay;
            while (musteri->hesaplar[sayac].id != -1)
            {
                toplamBakiye += musteri->hesaplar[sayac].bakiye;
                sayac++;
            }
            if (toplamBakiye >= tutar)
            {
                puts("Diger hesaplarinizda cekmek istediginiz kadar tutar bulunmaktadir.");
                puts("Onlari kullanip islem yapmak ister misiniz?");
                puts("Evet icin 1 hayir icin 0: ");
                scanf("%d", &onay);

                if (onay == 1)
                {
                    sayac = 0;
                    while (musteri->hesaplar[sayac].id != -1 && toplamCekilen != tutar)
                    {
                        if (musteri->hesaplar[sayac].bakiye >= tutar && musteri->hesaplar[sayac].bakiye != 0)
                        {
                            toplamCekilen = tutar;
                            dekontOlusturVeDosyayaKaydet(banka, musteri->hesaplar[sayac].id, tutar, musteri->hesaplar[sayac].id, ParaCekme, musteri->id, musteri->id);
                            bakiyeGuncelle(musteri->hesaplar + sayac, -tutar);
                            musteriHesabindakiDegisikligiBankayaYansit(banka, musteri->hesaplar + sayac);
                            break;
                        }
                        else if (musteri->hesaplar[sayac].bakiye != 0)
                        {
                            if (tutar - toplamCekilen > musteri->hesaplar[sayac].bakiye)
                            {
                                dekontOlusturVeDosyayaKaydet(banka, musteri->hesaplar[sayac].id, musteri->hesaplar[sayac].bakiye, musteri->hesaplar[sayac].id, ParaCekme, musteri->id, musteri->id);
                                toplamCekilen += musteri->hesaplar[sayac].bakiye;
                                bakiyeGuncelle(musteri->hesaplar + sayac, -musteri->hesaplar[sayac].bakiye);
                            }
                            else
                            {
                                bakiyeGuncelle(musteri->hesaplar + sayac, -(tutar - toplamCekilen));
                                dekontOlusturVeDosyayaKaydet(banka, musteri->hesaplar[sayac].id, tutar - toplamCekilen, musteri->hesaplar[sayac].id, ParaCekme, musteri->id, musteri->id);
                                toplamCekilen = tutar;
                            }
                            musteriHesabindakiDegisikligiBankayaYansit(banka, musteri->hesaplar + sayac);
                        }
                        sayac++;
                    }
                    hesaplardakiDegisikligiDosyayaKaydet(banka);
                }
            }
        }
        // para cekme islemi basariliysa bankanin hesaplarini da guncelle
        // ayni zamanda bu islemi dekont olarak kaydetmeliyiz
        else
        {
            musteriHesabindakiDegisikligiBankayaYansit(banka, musteri->hesaplar + counter);
            hesaplardakiDegisikligiDosyayaKaydet(banka);
            dekontOlusturVeDosyayaKaydet(banka, hesapId, tutar, hesapId, ParaCekme, musteri->id, musteri->id);
        }
    }
    else
    {
        printf("%d numarali bir hesabiniz yok. \n", hesapId);
    }
}

void paraYatir(Banka *banka, Musteri *musteri)
{
    ekraniTemizle();
    int hesapId;
    int tutar;
    printf("\n\tIslem yapmak istediginiz hesabinizin id'sini giriniz: ");
    scanf("%d", &hesapId);
    int counter = 0;
    // girilen hesap id musteriye ait mi?
    while (musteri->hesaplar[counter].id != hesapId && counter < 10)
    {
        counter++;
    }
    if (musteri->hesaplar[counter].id == hesapId)
    {
        printf("\n\tMiktar: ");
        scanf("%d", &tutar);
        int islem = bakiyeGuncelle(musteri->hesaplar + counter, tutar);
        musteriHesabindakiDegisikligiBankayaYansit(banka, musteri->hesaplar + counter);
        hesaplardakiDegisikligiDosyayaKaydet(banka);
        dekontOlusturVeDosyayaKaydet(banka, hesapId, tutar, hesapId, ParaYatirma, musteri->id, musteri->id);
    }
    else
    {
        printf("%d numarali bir hesabiniz yok.\n", hesapId);
    }
}

void hesabaHavaleYap(Banka *banka, Musteri *musteri)
{
    ekraniTemizle();
    int hesapId;
    int tutar;
    int islemUcreti = 0;
    int gondericiIslem = 0;
    int aliciIslem = 0;
    printf("\n\tIslem yapmak istediginiz hesabinizin numarasini giriniz: ");
    scanf("%d", &hesapId);
    int counterGonderici = 0;
    int counterAlici = 0;
    // girilen hesap id musteriye ait mi?
    while (musteri->hesaplar[counterGonderici].id != hesapId && counterGonderici < 10)
    {
        counterGonderici++;
    }
    if (musteri->hesaplar[counterGonderici].id == hesapId)
    {
        printf("\n\tMiktar: ");
        scanf("%d", &tutar);
        if (musteri->tip == Bireysel)
        {
            islemUcreti = tutar * 0.02;
            printf("%d tutarinda islem ucreti yansitiliyor.", islemUcreti);
        }
    }
    else
    {
        printf("%d numarali bir hesabiniz yok.", hesapId);
        return;
    }
    int aliciHesapNu;

    printf("\n\tAlici hesap nu:");
    scanf("%d", &aliciHesapNu);
    while (banka->hesaplar[counterAlici].id != aliciHesapNu && counterAlici < 100)
    {
        counterAlici++;
    }
    if (banka->hesaplar[counterAlici].id == aliciHesapNu)
    {

        gondericiIslem = bakiyeGuncelle(musteri->hesaplar + counterGonderici, -(tutar + islemUcreti));
        if (gondericiIslem)
        {
            musteriHesabindakiDegisikligiBankayaYansit(banka, musteri->hesaplar + counterGonderici);
            hesaplardakiDegisikligiDosyayaKaydet(banka);
            dekontOlusturVeDosyayaKaydet(banka, hesapId, tutar + islemUcreti, aliciHesapNu, HavaleGonderme, musteri->id, banka->hesaplar[counterAlici].musteriId);
        }
        else
        {
            puts("Islem basarisiz");
            return;
        }

        int islem = bakiyeGuncelle(banka->hesaplar + counterAlici, tutar);
        if (islem)
        {
            musteriHesabindakiDegisikligiBankayaYansit(banka, banka->hesaplar + counterAlici);
            hesaplardakiDegisikligiDosyayaKaydet(banka);
            dekontOlusturVeDosyayaKaydet(banka, banka->hesaplar[counterAlici].musteriId, tutar, aliciHesapNu, HavaleAlma, musteri->id, banka->hesaplar[counterAlici].musteriId);
        }
    }
    else
    {
        printf("Bankamizda %d numarali bir hesap kayitli degildir.", hesapId);
        return;
    }
}

void hesapOzetiGoruntule(Banka *banka, Musteri *musteri)
{
    ekraniTemizle();
    int gunBaslangic, gunBitis, ayBaslangic, ayBitis, yilBaslangic, yilBitis;

    int hesapId, counter = 0;

    printf("\n\tOzet goruntulemek istediginiz hesabi giriniz: ");
    scanf("%d", &hesapId);
    while (musteri->hesaplar[counter].id != hesapId && counter < 10)
    {
        counter++;
    }
    if (musteri->hesaplar[counter].id == hesapId)
    {
        printf("\nBaslangic icin: gun-ay-yil seklinde bir tarih giriniz, or-> 24-12-1886\n\tBaslangic tarihi:\t");
        Tarih baslangic;
        scanf("%d-%d-%d", &baslangic.gun, &baslangic.ay, &baslangic.yil);
        Tarih bitis;
        printf("\nBitis tarihini ayni formatta giriniz.\n\tBitis tarihi:\t");
        scanf("%d-%d-%d", &bitis.gun, &bitis.ay, &bitis.yil);
        counter = 0;
        while (banka->dekontlar[counter].hesapId != -1 && counter < 100)
        {
            if (banka->dekontlar[counter].hesapId == hesapId)
            {
                Tarih dekontTarihi;
                dekontTarihi.gun = banka->dekontlar[counter].gun;
                dekontTarihi.ay = banka->dekontlar[counter].ay;
                dekontTarihi.yil = banka->dekontlar[counter].yil;

                int bitisTarihindenOnceMi = tarih_karsilastir(dekontTarihi, bitis);
                int baslangicTarihindenSonraMi = tarih_karsilastir(dekontTarihi, baslangic);
                Musteri *alici, *gonderici;
                if (bitisTarihindenOnceMi <= 0 || baslangicTarihindenSonraMi >= 1)
                {
                    switch (banka->dekontlar[counter].islemTuru)
                    {
                    case ParaCekme:
                        printf("%d-%d-%d tarihinde %d tutarinda para cekme islemi.\n",
                               banka->dekontlar[counter].gun,
                               banka->dekontlar[counter].ay,
                               banka->dekontlar[counter].yil,
                               banka->dekontlar[counter].tutar);
                        break;
                    case ParaYatirma:
                        printf("%d-%d-%d tarihinde %d tutarinda para yatirma islemi.\n",
                               banka->dekontlar[counter].gun,
                               banka->dekontlar[counter].ay,
                               banka->dekontlar[counter].yil,
                               banka->dekontlar[counter].tutar);
                        break;
                    case HavaleGonderme:
                        alici = getMusteriById(banka->dekontlar[counter].aliciId);
                        printf("%d-%d-%d tarihinde %s %s kisisine %d tutarinda havale gonderme islemi.\n",
                               banka->dekontlar[counter].gun,
                               banka->dekontlar[counter].ay,
                               banka->dekontlar[counter].yil,
                               alici->ad,
                               alici->soyad,
                               banka->dekontlar[counter].tutar);
                        break;
                    case HavaleAlma:
                        alici = getMusteriById(banka->dekontlar[counter].aliciId);
                        printf("%d-%d-%d tarihinde %s %s kisisinden gelen %d tutarinda havale alma islemi.\n",
                               banka->dekontlar[counter].gun,
                               banka->dekontlar[counter].ay,
                               banka->dekontlar[counter].yil,
                               alici->ad,
                               alici->soyad,
                               banka->dekontlar[counter].tutar);
                        break;
                    default:
                        break;
                    }
                }
            }
            counter++;
        }
    }
    else
    {
        puts("Bu numaraya sahip bir hesabiniz yok.");
    }
}

void hesapKapama(Banka *banka, Musteri *musteri)
{
    ekraniTemizle();
    int hesapId, counter = 0;
    printf("\n\tKapatmak istediginiz hesabinizin numarasini giriniz: ");
    scanf("%d", &hesapId);
    while (musteri->hesaplar[counter].id != hesapId && counter < 10)
    {
        counter++;
    }
    if (musteri->hesaplar[counter].id == hesapId)
    {
        if (musteri->hesaplar[counter].bakiye == 0)
        {
            //bakiyeyi -2 yaparak silmis sayiyoruz ve dosyayi guncellerken dosyaya yazmiyoruz
            musteri->hesaplar[counter].bakiye = -2;
            musteriHesabindakiDegisikligiBankayaYansit(banka, musteri->hesaplar + counter);
            hesaplardakiDegisikligiDosyayaKaydet(banka);
            puts("Hesabiniz silindi...");
        }
        else
        {
            printf("Bakiyeniz(%dTL) 0TL olmadigindan hesabinizi kapatamadik.\n", musteri->hesaplar[counter].bakiye);
        }
    }
    else
    {
        puts("Boyle bir hesabiniz yok.");
    }
}

void musteriSecimEkrani(Banka *banka)
{
    ekraniTemizle();
    printf("\n\tMusteri id'sini giriniz: ");
    int id = 0;
    scanf("%d", &id);
    Musteri *musteri = getMusteriById(id);
    if (musteri != NULL)
    {
        //kisi seciminden sonraki islemler
        int secim = 0;
        while (secim != 3)
        {
            printf("Secili Musteri %s %s\n", musteri->ad, musteri->soyad);
            puts("1- Hesap Olustur");
            puts("2- Para Cek");
            puts("3- Para Yatir");
            puts("4- Havale Yap");
            puts("5- Hesap Ozeti");
            puts("6- Hesap kapama");
            puts("7- Cikis");

            scanf("%d", &secim);
            switch (secim)
            {
            case 1:
                hesapOlustur(banka, musteri);
                break;
            case 2:
                paraCek(banka, musteri);
                break;
            case 3:
                paraYatir(banka, musteri);
                break;
            case 4:
                hesabaHavaleYap(banka, musteri);
                break;
            case 5:
                hesapOzetiGoruntule(banka, musteri);
                break;
            case 6:
                hesapKapama(banka, musteri);
                break;
            case 7:
                return;
            default:
                puts("yanlis giris yaptiniz");
                break;
            }
        }
    }
    else
    {
        puts("Boyle bir musteri bulunmamaktadir.");
    }
}

void anaEkran(Banka *banka)
{
    ekraniTemizle();
    puts("Banka otomasyonuna hosgeldiniz.");
    while (1)
    {
        puts("1- Musteri Ekle");
        puts("2- Musteri Sec");
        //puts("3- Kisi Arama");
        puts("3- Cikis");
        int a;
        scanf("%d", &a);
        switch (a)
        {
        case 1:
            musteriOlustur(banka);
            break;
        case 2:
            musteriSecimEkrani(banka);
            break;
        /*            
        case 3:
            for (int a = 0; a < MAX_BANKA_MUSTERI; a++)
            {
                if (banka->musteriler[a].id != -1)
                {
                    printf("%s %s, ", banka->musteriler[a].ad, banka->musteriler[a].soyad);
                }
            }
            break; */
        case 3:
            return;
        default:
            ekraniTemizle();
            puts("yanlis giris yaptiniz");
        }
    }
}

int main(void)
{
    GUN = get_tarih(0);
    AY = get_tarih(1);
    YIL = get_tarih(2);

    Banka *banka = (Banka *)malloc(sizeof(Banka));

    bankaBaslangicBilgileriDosyadanOku(banka);

    anaEkran(banka);
    return 0;
}