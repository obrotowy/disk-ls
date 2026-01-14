# disk-ls
Proste demo na potrzeby przedmiotu Oprogramowanie Systemowe.

# Zaimplementowane rzeczy
- [x] Enumeracja partycji w MBR
- [ ] Enumeracja partycji w GPT
- [ ] Enumeracja plików FAT32
- [ ] Enumeracja plików EXT

# Użycie
```
make
./disk-ls disk.bin
```

## Jak utworzyć obraz dysku?
```
dd if=/dev/zero of=disk.bin count=<rozmiar w MB> bs=1M
cfdisk disk.bin
```

## Jak formatować i montować utworzone partycje?
Z `fdisk -l disk.bin` odczytujemy offset wybranej partycji, mnożymy go przez rozmiar sektora (domyślnie 512 bajtów)

```
sudo losetup -f --show -o <offset_w_bajtach> disk.bin
```

Z utworzonym przez powyższe polecenie plikiem obchodzimy się jak ze standardową partycją np:
```
mkdosfs -F 32 /dev/loop0          ; w celu sformatowania w FAT32
mount /dev/loop0 /mnt/demo        ; w celu zamontowania
echo "test" > /mnt/demo/test.txt  ; w celu zapisania pliku
```

Aby usunąć plik robimy
```
sudo losetup -d /dev/loop0
```
