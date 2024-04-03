create database QLBongDa; 
use QLBongDa
create table CAUTHU (
	MACT numeric primary key identity(1,1),
	HOTEN nvarchar(100) not null,
	VITRI nvarchar(20) not null,
	NGAYSINH datetime,
	DIACHI nvarchar(200),
	MACLB varchar(5) not null,
	MAQG varchar(5) not null,
	SO int not null
)

create table QUOCGIA(
	MAQG varchar(5) primary key,
	TENQG nvarchar(60) not null
)

create table CAULACBO(
	MACLB varchar(5) primary key,
	TENCLB nvarchar(100) not null,
	MASAN varchar(5) not null,
	MATINH varchar(5) not null
)

create table TINH(
	MATINH varchar(5) primary key,
	TENTINH nvarchar(100) not null
)

create table SANVD (
	MASAN varchar(5) primary key,
	TENSAN nvarchar(100) NOT NULL,
	DIACHI nvarchar(200),
)

create table HUANLUYENVIEN (
	MAHLV varchar(5) primary key,
	TENHLV nvarchar(100) not null,
	NGAYSINH datetime,
	DIACHI nvarchar(200),
	DIENTHOAI nvarchar(20),
	MAQG varchar(5) not null
)

create table HLV_CLB (
	MAHLV varchar(5),
	MACLB varchar(5),
	VAITRO nvarchar(100) not null,
	primary key(MAHLV, MACLB)
)

create table TRANDAU (
	MATRAN numeric primary key identity(1,1),
	NAM int NOT NULL,
	VONG int NOT NULL,
	NGAYTD datetime NOT NULL,
	MACLB1 varchar(5) NOT NULL,
	MACLB2 varchar(5) NOT NULL,
	MASAN varchar(5) NOT NULL,
	KETQUA varchar(5) NOT NULL,
)

create table BANGXH (
	MACLB varchar(5),
	NAM int, 
	VONG int, 
	SOTRAN int not null,
	THANG int not null,
	HOA int not null,
	THUA int not null,
	HIEUSO varchar(5) not null,
	DIEM int not null,
	HANG int not null,
	primary key(MACLB,NAM,VONG)
)

insert into CAUTHU 
	(HOTEN, VITRI, NGAYSINH, DIACHI, MACLB, MAQG, SO)
values
	(N'Nguyễn Vũ Phong', N'Tiền vệ', '1990/02/20', NULL, 'BBD', 'VN', 17),
	(N'Nguyễn Công Vinh', N'Tiền đạo', '1992/03/10', NULL, 'HAGL', 'VN', 9),
	(N'Trần Tấn Tài', N'Tiền vệ', '1989/11/12', NULL, 'BBD', 'VN', 8),
	(N'Phan Hồng Sơn', N'Thủ môn', '1991/06/10', NULL, 'HAGL', 'VN', 1),
	(N'Ronaldo', N'Tiền vệ', '1989/12/12', NULL, 'SDN', 'BRA', 7),
	(N'Robinho', N'Tiền vệ', '1989/10/12', NULL, 'SDN', 'BRA', 8),
	(N'Vidic', N'Hậu vệ', '1987/10/15', NULL, 'HAGL', 'ANH', 3),
	(N'Trần Văn Santos', N'Thủ môn', '1990/10/21', NULL, 'BBD', 'BRA', 1)


insert into QUOCGIA
values
	('VN', N'Việt Nam'),
	('ANH', N'Anh Quốc'),
	('TBN', N'Tây Ban Nha'),
	('BDN', N'Bồ Đào Nha'),
	('BRA', 'Brazil'),
	('ITA', N'Ý'),
	('THA', N'Thái Lan')

insert into CAULACBO
values
	('BBD', N'BECAMEX BÌNH DƯƠNG', 'GD', 'BD'),
	('HAGL', N'HOÀNG ANH GIA LAI', 'PL', 'GL'),
	('SDN', N'SHB ĐÀ NẴNG', 'CL', 'DN'),
	('KKH', N'KHATOCO KHÁNH HÒA', 'NT', 'KH'),
	('TPY', N'THÉP PHÚ YÊN', 'TH', 'PY'),
	('GDT', N'GẠCH ĐỒNG TÂM LONG AN', 'LA', 'LA')

insert into TINH
values
	('BD', N'Bình Dương'),
	('GL', N'Gia Lai'),
	('DN', N'Đà Nẵng'),
	('KH', N'Khánh Hòa'),
	('PY', N'Phú Yên'),
	('LA', N'Long An')

insert into SANVD 
values
	('GD', N'Gò Đậu', N'123 QL1, TX Thủ Dầu Một, Bình Dương'),
	('PL', N'Pleiku', N'22 Hồ Tùng Mậu, Thống Nhất, Thị xã Pleiku, Gia Lai'),
	('CL', N'Chi Lăng', N'127 Võ Văn Tần, Đà Nẵng'),
	('NT', N'Nha Trang', N'128 Phan Chu Trinh, Nha Trang, Khánh Hòa'),
	('TH', N'Tuy Hòa', N'57 Trường Chinh, Tuy Hòa, Phú Yên'),
	('LA', N'Long An', N'102 Hùng Vương, Tp Tân An, Long An')

insert into HUANLUYENVIEN
values
	('HLV01', N'Vital', '1955/10/15', NULL, '0918011075', 'BDN'),
	('HLV02', N'Lê Huỳnh Đức', '1972/05/20', NULL, '01223456789', 'VN'),
	('HLV03', N'Kiatisuk', '1970/12/11', NULL, '01990123456', 'THA'),
	('HLV04', N'Hoàng Anh Tuấn', '1970/06/10', NULL, '0989112233', 'VN'),
	('HLV05', N'Trần Công Minh', '1973/07/07', NULL, '0909099990', 'VN'),
	('HLV06', N'Trần Văn Phúc', '1965/03/02', NULL, '01650101234', 'VN')

insert into HLV_CLB
values
	('HLV01', 'BBD', N'HLV Chính'),
	('HLV02', 'SDN', N'HLV Chính'),
	('HLV03', 'HAGL', N'HLV Chính'),
	('HLV04', 'KKH', N'HLV Chính'),
	('HLV05', 'GDT', N'HLV Chính'),
	('HLV06', 'BBD', N'HLV thủ môn')

insert into TRANDAU
	(NAM, VONG, NGAYTD, MACLB1, MACLB2, MASAN, KETQUA)
values 
	('2009', 1, '2009/2/7', 'BBD', 'SDN', 'GD', '3-0'),
	('2009', 1, '2009/2/7', 'KKH', 'GDT', 'NT', '1-1'),
	('2009', 2, '2009/2/16', 'SDN', 'KKH', 'CL', '2-2'),
	('2009', 2, '2009/2/16', 'TPY', 'BBD', 'TH', '5-0'),
	('2009', 3, '2009/3/1', 'TPY', 'GDT', 'TH', '0-2'),
	('2009', 3, '2009/3/1', 'KKH', 'BBD', 'NT', '0-1'),
	('2009', 4, '2009/3/7', 'KKH', 'TPY', 'NT', '1-0'),
	('2009', 4, '2009/3/7', 'BBD', 'GDT', 'GD', '2-2')

insert into BANGXH
values
	('BBD', '2009', 1, 1, 1, 0, 0, '3-0', 3, 1),
	('KKH', '2009', 1, 1, 0, 1, 0, '1-1', 1, 2),
	('GDT', '2009', 1, 1, 0, 1, 0, '1-1', 1, 3),
	('TPY', '2009', 1, 0, 0, 0, 0, '0-0', 0, 4),
	('SDN', '2009', 1, 1, 0, 0, 1, '0-3', 0, 5),
	('TPY', '2009', 2, 1, 1, 0, 0, '5-0', 3, 1),
	('BBD', '2009', 2, 2, 1, 0, 1, '3-5', 3, 2),
	('KKH', '2009', 2, 2, 0, 2, 0, '3-3', 2, 3),
	('GDT', '2009', 2, 1, 0, 1, 0, '1-1', 1, 4),
	('SDN', '2009', 2, 2, 1, 1, 0, '2-5', 1, 5),
	('BBD', '2009', 3, 3, 2, 0, 1, '4-5', 6, 1),
	('GDT', '2009', 3, 2, 1, 1, 0, '3-1', 4, 2),
	('TPY', '2009', 3, 2, 1, 0, 1, '5-2', 3, 3),
	('KKH', '2009', 3, 3, 0, 2, 1, '3-4', 2, 4),
	('SDN', '2009', 3, 2, 1, 1, 0, '2-5', 1, 5),
	('BBD', '2009', 4, 4, 2, 1, 1, '6-7', 7, 1),
	('GDT', '2009', 4, 3, 1, 2, 0, '5-1', 5, 2),
	('KKH', '2009', 4, 4, 1, 2, 1, '4-4', 5, 3),
	('TPY', '2009', 4, 3, 1, 0, 2, '5-3', 3, 4),
	('SDN', '2009', 4, 2, 1, 1, 0, '2-5', 1, 5)

alter table TRANDAU
	add foreign key (MASAN) references SANVD(MASAN),
		foreign key (MACLB1) references CAULACBO(MACLB),
		foreign key (MACLB2) references CAULACBO(MACLB);

alter table BANGXH
	add foreign key (MACLB) references CAULACBO(MACLB);

alter table CAULACBO
	add foreign key (MATINH) references TINH(MATINH),
		foreign key (MASAN) references SANVD(MASAN)

alter table CAUTHU
	add foreign key (MACLB) references CAULACBO(MACLB),
		foreign key (MAQG) references QUOCGIA(MAQG)

alter table HUANLUYENVIEN
	add foreign key (MAQG) references QUOCGIA(MAQG)

alter table HLV_CLB
	add foreign key (MAHLV) references HUANLUYENVIEN(MAHLV),
		foreign key (MACLB) references CAULACBO(MACLB)

drop database QLBongDa