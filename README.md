# BankaOtomasyonu
C dilinde yazılmış, dosya işlemleri içeren basit bir banka otomasyonu proje ödevi

 
Banka Otomasyon Sistemi – Proje Metni 
Sizden bir Banka Otomasyonu gerçekleştirmeniz istenmektedir.   
1.	Yeni Müşteri Ekleme; Banka işlemlerini yapacak müşterilerin sisteme eklenmesi işlemi gerçekleştirilecektir. Sisteme ekleme işleminde müşteri bilgileri (ad, soyad, musteri no, müşteri tipi, hesaplar, doğum yeri, doğum tarihi vb.) bir dosyaya yazılacaktır. Aynı müşteri numarası başka müşteriler tarafından kullanılamayacaktır. Müşteri numaraları random olarak atanacaktır. Ayrıca farklı ayrıcalıklara sahip olan 2 tip müşteri bulunmaktadır. Müşteriler; ticari müşteri ve bireysel müşteri olabilmektedir. Bireysel ve ticari müşterilerin bilgileri ayrı dosyalarda tutulmalıdır. 
2.	Hesap Açtırma; 
     * Bir müşterinin birden fazla hesabı olabilir. 
     * Her hesabı için ayrı ayrı hesap numarası verilmesi gerekir. Aynı hesap numarası birden fazla müşteriye verilmemelidir.  
3.	Para Çekme; Müşteri, para çekme işlemi gerçekleştirecekse hesap ve tutar bilgisi girilip tutar kadar değer bakiyesinden düşülmelidir. Eğer bakiyesinde yeterli tutar bulunmuyorsa, ek hesabını kullanarak para çekme işlemini gerçekleştirmelidir. Günlük maksimum para çekme limiti bireysel müşteriler için 750 TL, ticari müşteriler için 1500 TL’dir. Daha fazla para çekilmek istendiği takdirde işlem gerçekleştirilmeyecektir. Günlük hareketler sonucunda güncellenen hesap bilgileri dosyada da güncellenecektir. 
4.	Para Yatırma; Müşteri, para yatırma işlemi gerçekleştirecekse hesap ve yatırılan tutar bilgisi girilip tutar kadar bakiyesi arttırılacaktır. Dosya üzerinde de gerekli güncellemeler yapılacaktır.  
5.	Hesaba Havale; Müşteri, havale yapacaksa; havale yapacağı kişinin hesabının kayıtlı olması gerekmektedir. Ardından havale tutarı kadar miktar kendi hesabının bakiyesinden eksilecek, gönderdiği kişinin bakiyesi artacaktır. Ayrıca Ticari Müşterilerden havale ücreti kesilmez iken, Bireysel Müşterilerden gönderilecek tutarın %2 oranında havale ücreti kesilmektedir. 
6. Hesap Özeti; Seçilen bir hesap için belirtilen tarih aralığında hesap özeti görüntülenecektir. Çekilen, yatırılan, havale yapılmışsa kime yapıldığı ve miktarı, başka bir hesaptan havale para geldiyse kimden geldiği ve miktarı gibi bilgiler ve bu işlemlerin tarihleri görüntülenmelidir. Ve dekont.txt isimli dosyaya yazılacaktır. 
7. Hesap Kapama; İstenilen hesap kapatılabilecektir. Kapama işlemi için hesap bakiyesi 0 olması gerekmektedir. İstenilen hesap kapatıldığında müşteri bilgileri bulunduğu dosyadan silinecektir. 
Önemli: 
1.	Dokümanda projenin sadece anahatları belirtilmiş olup, senaryoyu mantıklı olmak kaydı ile genişletebilirsiniz. 
2.	Projenizde kullanılması gerekli olan yerlerde struct yapısı kullanmanız gerekmektedir. (Örn: müşteri, hesap, banka vb.) 
3.	Tüm dizi elemanları üzerinde yapılan işlemlerde pointer kullanmanız gerekmektedir. 
4.	Uygulamanızın bir ana menüsü olmalıdır. (Müşteri ekle, müşteri seç, seçilen müşteri için para çek, seçilen müşteri için para yatır vb.) 
