=================================================================================
Canon UFR II Printer Driver for Linux Version 2.40

PLEASE READ THIS DOCUMENT CAREFULLY
=================================================================================


---------------------------------------------------------------------------------
Trademarks

Adobe, Acrobat, Acrobat Reader, PostScript and PostScript 3 are trademarks of
Adobe Systems Incorporated.
Linux is a trademark of Linus Torvalds.
OpenOffice.org and the OpenOffice.org logo are trademarks or registered trademarks 
of Oracle and/or its affiliates.
HP-GL is a trademark of Hewlett-Packard Company.
UNIX is a trademark of The Open Group in the United States and other countries.
Other product and company names herein may be the trademarks of their respective
owners.
---------------------------------------------------------------------------------


---------------------------------------------------------------------------------
CONTENTS

Before Starting
1. Introduction
2. Distribution File Structure of the Canon UFR II Printer Driver for Linux
3. Hardware Requirements
4. Cautions, Limitations, and Restrictions
---------------------------------------------------------------------------------


1. Introduction -----------------------------------------------------------------
Thank you for using the Canon UFR II/UFR II LT Printer Driver for Linux. This printer 
driver provides printing functions for Canon LBP/imageRUNNER ADVANCE/Color 
imageRUNNER/imageRUNNER/imagePRESS/MF series products operating under the CUPS 
(Common Unix Printing System) environment, a printing system that functions on Linux 
operating systems.


2. Distribution File Structure of the Canon UFR II Printer Driver for Linux -----
The Canon UFR II Printer Driver for Linux distribution files are as follows:
Furthermore, the file name for the CUPS driver common module and printer driver
module differs depending on the version.

- README-ufr2-2.4xUK.txt (This document)
Describes supplementary information on the Canon UFR II Printer Driver for Linux.

- LICENSE-ufr2drv-2.4x.txt
Describes User License Agreement on the Canon UFR II Printer Driver for Linux.

- guide-ufr2-2.4xUK.tar.gz
Online manual that explains how to use the Canon UFR II Printer Driver for Linux.
This includes the system requirements, installation, and usage of the Canon UFR II
Printer Driver for Linux.
Because this file is in a compressed format, you need to extract it to the
appropriate directory before reading.

- cndrvcups-common-2.40-X.i386.rpm (for 32-bit)
- cndrvcups-common-2.40-X.x86_64.rpm (for 64-bit)
- cndrvcups-common_2.40-X_i386.deb (for Debian 32-bit)
Installation package for the CUPS driver common module used by the Canon UFR II
Printer Driver for Linux.

- cndrvcups-ufr2-uk-2.40-X.i386.rpm (for 32-bit)
- cndrvcups-ufr2-uk-2.40-X.x86_64.rpm (for 64-bit)
- cndrvcups-ufr2-uk_2.40-X_i386.deb (for Debian 32-bit)
Installation package for the Canon UFR II Printer Driver for Linux.

- cndrvcups-common-2.40-X.tar.gz
Source file for the CUPS driver common module used by the Canon UFR II Printer 
Driver for Linux.

- cndrvcups-lb-2.40-X.tar.gz
Source file for the Canon UFR II Printer Driver for Linux.


3. Hardware Requirements --------------------------------------------------------
This printer driver can be used with the following hardware environment.

Hardware: Computer that is enable to operate Linux, with x86 compatible CPU
          (32-bit or 64-bit)

Evaluated OS:
          Fedora 15  32-bit/64-bit
          Ubuntu 11.04 Desktop 32-bit

Previously Evaluated OS:
          Turbolinux 10 Desktop  32-bit
          Turbolinux 10 F...  32-bit
          Turbolinux 10 S  32-bit
          Turbolinux Version 11 FUJI  32-bit
          Turbolinux Home  32-bit
          Turbolinux 10 Server x64 Edition  64-bit
          MIRACLE LINUX V3.0 (Asianux Inside)  32-bit/64-bit
          MIRACLE LINUX V4.0 (Asianux Inside)  32-bit/64-bit
          Red Hat 9  32-bit
          Red Hat Professional Workstation  32-bit
          Red Hat Enterprise Linux v.4  32-bit/64-bit
          Red Hat Enterprise Linux v.5  32-bit/64-bit
          Mandriva Linux One 2008  32-bit
          Mandriva Linux One 2008 Spring  32-bit
          Mandriva Linux PowerPack 2008  32-bit/64-bit
          Mandriva Linux PowerPack 2008 Spring  32-bit/64-bit
          Mandriva Linux One 2009  32-bit
          SUSE LINUX PROFESSIONAL 9.3  32-bit
          Novell Linux Desktop 9  32-bit
          SUSE Linux 10.0 (openSUSE)  32-bit/64-bit
          SUSE Linux 10.1 (openSUSE)  32-bit/64-bit
          SUSE Linux 10.2 (openSUSE)  32-bit/64-bit
          SUSE Linux 10.3 (openSUSE)  32-bit/64-bit
          SUSE Linux 11.0 (openSUSE)  32-bit/64-bit
          SUSE Linux 11.1 (openSUSE)  32-bit/64-bit
          Fedora Core 4  32-bit/64-bit
          Fedora Core 5  32-bit/64-bit
          Fedora Core 6  32-bit/64-bit
          Fedora 7  32-bit/64-bit
          Fedora 8  32-bit/64-bit
          Fedora 9  32-bit/64-bit
          Fedora 10  32-bit/64-bit
          Fedora 11  32-bit/64-bit
          Fedora 12  32-bit/64-bit
          Fedora 13  32-bit/64-bit
          Fedora 14  32-bit/64-bit
          Ubuntu 7.04 Desktop  32-bit
          Ubuntu 7.10 Desktop  32-bit
          Ubuntu 8.04 Desktop  32-bit
          Ubuntu 8.10 Desktop  32-bit
          Ubuntu 9.04 Desktop  32-bit
          Ubuntu 9.10 Desktop  32-bit
          Ubuntu 10.04 Desktop  32-bit
          Ubuntu 10.10 Desktop  32-bit
          Debian GNU/Linux 3.1 rev2  32-bit
          Debian GNU/Linux 4.0  32-bit
          Debian GNU/Linux 4.0r6 etchnhalf  32-bit
          Debian GNU/Linux 5.02  32-bit
          Vine Linux 3.1/3.1CR  32-bit
          Vine Linux 4.1  32-bit
          Vine Linux 4.2  32-bit
          CentOS 5.3  32-bit/64-bit

Supported Printer: LBP3360
                   LBP3370
                   LBP3460
                   LBP5360
                   LBP5960
                   LBP5970
                   LBP5975
                   LBP6650dn
                   LBP6750/3560
                   LBP7660C
                   LBP7680C/5280
                   LBP7750C
                   imageRUNNER ADVANCE C2020/2030
                   imageRUNNER ADVANCE C2020/2030i
                   imageRUNNER ADVANCE C2020/2030L
                   imageRUNNER ADVANCE C2025
                   imageRUNNER ADVANCE C5030/C5030i/C5035/C5035i
                   imageRUNNER ADVANCE C5051/C5051i/C5045/C5045i
                   imageRUNNER ADVANCE C7055/C7065
                   imageRUNNER ADVANCE C9060/C9070 PRO
                   imageRUNNER ADVANCE C9065/C9075 PRO
                   imageRUNNER ADVANCE 6055/6055i/6065/6065i
                   imageRUNNER ADVANCE 6075/6075i
                   imageRUNNER ADVANCE 8085/8095
                   imageRUNNER ADVANCE 8105
                   imageRUNNER1730/1740/1750
                   imageRUNNER2520/2520i
                   imageRUNNER2525/2525i/2530/2530i
                   imageRUNNER2535/2535i/2545/2545i
                   iR105+
                   iR1018
                   iR1020
                   iR1022
                   iR1024/1024A/1024F/1024i/1024iF
                   iR1133
                   iR2016/2016i
                   iR2018/2018i
                   iR2020/2020i
                   iR2022
                   iR2025
                   iR2030
                   iR2230
                   iR2270
                   iR2318L
                   iR2320L/2320N
                   iR2420D/2420L
                   iR2422D/2422J/2422L/2422N
                   iR2830
                   iR2870
                   iR3025
                   iR3030
                   iR3035
                   iR3045
                   iR3225/3225N
                   iR3230/3230A/3230N
                   iR3235/3235A/3235N
                   iR3245/3245A/3245N
                   iR3530
                   iR3570
                   iR4530
                   iR4570
                   iR5055
                   iR5065
                   iR5075
                   iR5570
                   iR6570
                   iR7086
                   iR7095/7095P
                   iR7105
                   iR8070
                   iR85+
                   iR9070
                   iR C1021/C1021i
                   iR C1028/C1030
                   iR C2380i
                   iR C2550/C2550i
                   iR C2580i
                   iR C2880/C2880i
                   iR C3080/C3080i
                   iR C3180/C3180i
                   iR 3180C/3180Ci
                   iR C3380/C3380i
                   iR C3580/C3580i
                   iR C4080
                   iR C4580
                   iR C5180
                   iR C5185
                   iR C5870
                   iR C5880/C5880i
                   iR 5880C/5880Ci
                   iR C6870
                   iR C6880/C6880i
                   iR 6880C/6880Ci
                   imagePRESS C1
                   imagePRESS C1+
                   D400-450
                   D460-490
                   D500 Series
                   D1100 Series
                   D1300/MF6700
                   MF3010
                   MF4010 Series
                   MF4100 Series
                   MF4200 Series
                   MF4320-4350
                   MF4360-4390
                   MF4400w
                   MF4400 Series
                   MF4500w
                   MF4500 Series
                   MF4600 Series
                   MF5800 Series
                   MF5900 Series
                   MF6500 Series
                   MF6600 Series
                   MF7100 Series
                   MF8000C
                   MF8000 Series
                   MF8300C
                   MF8300 Series
                   MF8400 Series
                   MF9100 Series
                   MF9200 Series
                   MF9300 Series
                   L160
                   L3000 Series

Please see the online manual about the install method and the concrete usage.


4. Cautions, Limitations, and Restrictions -------------------------------------

- If you install "cndrvcups-common" package version 2.40, make sure you install 
  the same version of the "cndrvcups-ufr2" package, i.e. 2.40.

- Update install is not available when you update to the version 1.40 or later, 
  as the file names are not consistent with the older versions. If you have 
  older version on your computer, you have to uninstall the older driver then 
  install the new driver.

- If more than two drivers are installed using the LIPSLX, UFRII(US), and UFRII
  (UK) driver install packages, and only one of these drivers is updated, an 
  error will occur.
  This problem can be solved by updating all of the installed drivers together.

- When specifying multiple pages/copies for [Page Layout] in the [General] 
  sheet to print a document created with StarSuite7/OpenOffice, due to a cause 
  of operation by the CUPS module, settings are not correctly assigned to the 
  multiple pages and output.

- PostScript files created with the number of copies specified in OpenOffice.org 
  or StarSuite are affected not by the value specified by [Number of copies] in 
  the [cngplp] dialog box (the driver UI), but by the number of copies set when 
  creating the PostScript file.

- If settings are changed from the driver UI, during print processing, the 
  printed result will reflect the changed settings.

- If [Brightness and Gamma] is specified in the [General] sheet from an 
  application such as OpenOffice.org, GIMP, or Acrobat Reader v.5.0, the settings 
  will be invalid.

- You cannot print a PDF document by directly specifying it from the desktop or 
  command line. When printing a PDF document, it is recommended that you print 
  it from Acrobat Reader or Adobe Reader.

- The maximum number of files that can be held in the print queue when printing 
  is 500 according to CUPS specifications. Files queued after the 500th file will 
  be ignored.

- If you are using SUSE LINUX Professional 9.3, the driver UI may display 
  unintelligible characters. You can solve this problem using the following 
  method.
  1) Log in as 'root'.
  2) Execute the following command to change the GTK+ environment settings.
     # cd /etc/
     # ln -s opt/gnome/gtk ./

- If you are using SUSE LINUX Professional 9.3, a warning may occur when you 
  activate the driver UI. You can solve this problem using the following 
  method.
  1) Open [K Menu] -> [Control Center].
  2) Select [Appearance & Themes].
  3) Select [Colors].
  4) Deselect [Apply colors to non-KDE applications].
  5) Close [Control Center].

- When printing from OpenOffice.org, some finishing functions such as [Rotate] 
  may not operate correctly if you specify multiple copies for [Number of 
  copies] in the driver UI.

- If you are using SUSE Linux 9.3 or SUSE Linux 10.0, and are printing from the 
  [Print] dialog box of Mozilla or FireFox, because the multiple copies setting 
  is not enabled, you can print only one copy regardless of how many copies you 
  have specified. This problem can be solved by changing the following line in 
  the file "/etc/cups/mime.convs".
  [Before change]
   application/mozilla-ps application/postscript 33 pswrite
  [After change]
   application/mozilla-ps application/postscript 33 pstops

- When performing banner printing in Fedora 8 or Fedora 9, if you specify a 
  setting other than [none] for [End] under [Banner] in the [General] sheet, 
  the print queue will stop.

- If you attempt to print from a Color iR/LBP printer using this driver in a 
  64-bit version of Fedora 9, Fedora 10, Fedora 11, Fedora 12, Fedora 13, 
  Fedora 14,or Fedora 15 system environment, printing results may not be 
  satisfactory.
  This is because when the 64-bit packages are installed by the standard 
  installation method, the 32-bit JPEG library is not installed.
  This problem can be solved by installing the package as an additional 
  installation.
  Execute the following command.
    <Fedora 9/10> # yum install libjpeg.i386
    <Fedora 11>   # yum install libjpeg.i586
    <Fedora 12/13>   # yum install libjpeg.i686
    <Fedora 14/15>   # yum install libjpeg-turbo.i686

- To install the common module in the 64-bit version of Fedora 10, Fedora 11, 
  Fedora 12, Fedora 13, Fedora 14, or Fedora 15, you need the 32-bit version 
  glibc library.
  You can install the glibc library by executing the following command:
    <Fedora 10> # yum install glibc.i386
    <Fedora 11> # yum install glibc.i586
    <Fedora 12/13/14/15> # yum install glibc.i686

  Also, printer drivers from version 1.90 onward require the 32-bit version 
  of the xml2 library.
  You can enable printing by executing the following command:
    <Fedora 10> # yum install libxml2.i386
    <Fedora 11> # yum install libxml2.i586
    <Fedora 12/13/14/15> # yum install libxml2.i686

- If you are using Fedora 11 and print with the print queue stopped after 
  canceling a job, the job is suspended. In this case, click the [Maintenance] 
  button in Printers in the CUPSWeb interface and select [Resume Printer] to 
  perform the [pending since] job again.
  If you cannot find the [Maintenance] button, you can select [Resume Printer] 
  by selecting [Pause Printer].

- If you are using OpenSUSE 10.2 or SLED10SP1, which includes Ghostscript 
  version 8.15.3, you may not be able to print some documents. To solve this 
  problem, install another version of Ghostscript.

- If you are using OpenSUSE 11.0 with Ghostscript version 8.6.x, printing from 
  Evince, GIMP, or other applications may take time.

- Although Okular is the standard document viewer in the OpenSUSE 11.1 KDE 
  environment, the printer driver functions do not operate correctly with it.
  You can solve this problem by using another application such as Acrobat 
  Reader etc.

- If you are using Ubuntu 7.04/7.10/8.04/8.10/9.04 or Debian 3.1/4.0/5.0, 
  the libcupsys2 library is required to install the common module.
  You can install the libcupsys2 library by executing the following command.
    # apt-get install libcupsys2

- If you are using Ubuntu 8.10, Ubuntu 9.04, Ubuntu 9.10, Ubuntu 10.04, Ubuntu 
  10.10, or Ubuntu 11.04, the printer will print with the default paper output 
  method, regardless of whether you have specified the paper output method.
  This problem can be solved by changing the output paper method setting from 
  the CUPS printer settings (Web).

- If you are using Ubuntu 8.10, specifying reverse order for printing does not 
  affect the print result.
  This problem can be solved by updating CUPS.

- If you are using Ubuntu 8.10, Ubuntu 9.04, Ubuntu 9.10, Ubuntu 10.04, Ubuntu 
  10.10, or Ubuntu 11.04, when printing PDF data or PS data, brightness and 
  gamma correction settings may not be reflected in the print result.

- If you are using Adobe Reader 7.0.x, and modify such settings as Paper Size, 
  Paper Source, Duplex Printing, etc. in the print dialog window, these options 
  are automatically added to the printer command. However, these settings will 
  not work because they cannot be recognized as command options. To solve this 
  problem, use "-o" to separate each command options.
     [before]  -o InputSlot=Manual,Duplex=DuplexNotumble 
     [after]   -o InputSlot=Manual -o Duplex=DuplexNoTumble

- When printing PDF files using Adobe Reader 8, there may be instances where 
  some image data is not printed.
  This problem may be solved by printing using Adobe Reader 7 or Adobe Reader 9, 
  or setting Level 3 in the PostScript options.

- When performing 2-sided printing with Adobe Reader 8.1.2, if you specify 
  [ON (Short-edged Binding)] for [Duplex Printing] in the print properties for 
  Adobe Reader 8.1.2, the document will be printed on both sides with long-edged 
  binding.
  This problem can be avoided by printing the document using the printer driver UI.

- If you are using Vine Linux 3.1, you may take time to print from Adobe Reader 
  7.0.9 or may not be able to print some documents.

- When printing PDF files containing Japanese characters from the command line 
  in Vine Linux 4.1, there may be instances where Ghostscript terminates 
  unexpectedly, causing printing to stop.
  This problem can be avoided by printing PDF files using Adobe Reader.

- When printing PDF files from Adobe Reader 8 in Vine Linux 4.1, there may be 
  instances where Ghostscript terminates unexpectedly, causing the print queue 
  to stop.
  This is caused by Ghostscript (7.07) not being able to analyze PS files 
  created by Adobe Reader 8, and consequently terminating prematurely, thereby 
  stopping the filtering process.
  This problem can be avoided by using Adobe Reader 7.

- When printing text files in landscape orientation in Vine Linux 4.1, Vine Linux 
  4.2, Fedora 8, Fedora 9, or Red Hat Enterprise Linux v.5, there may be instances 
  where the text file is printed in portrait orientation with some of the print 
  data not being printed on the page.
  This is caused by the CUPS filter employed by the distribution you are using 
  creating a PS command that is already set to portrait. 
  Also, some of the functions provided in the CUPS standard filter "texttops" 
  may not operate correctly.
  This problem can be avoided by changing the CUPS filter name specified in 
  the "text/plain" entry line in the CUPS setting file "mime.convs" to the CUPS 
  standard filter "texttops". This will result in Japanese characters being 
  misprinted, therefore when printing Japanese characters, it is necessary to 
  print a PS command created with a text editor or text/PostScript conversion 
  program such as paps.

- If you specify Paper Source settings in the print dialog of an application 
  such as Writer of OpenOffice.org, the settings made from the application are 
  overridden by the printer driver UI settings. To print from the desired 
  Paper Source, specify the Paper Source from the printer driver UI beforehand, 
  or print from the command line.

- If you are using Debian GNU/Linux 4.0, a PPD file error may occur when you 
  register the printer (PPD) with the print spooler. To solve this problem, 
  use "-P (full path to the ppd)" instead of "-m" when you specify the ppd 
  using the command line.
    Example: /usr/sbin/lpadmin -p iRC5180
              -P /usr/share/cups/model/CNCUPSIRC5180ZK.ppd
              -v lpd://192.168.1.10/iRC5180 -E

- In Ubuntu 10.10, Ubuntu 11.04, Fedora 14, or Fedora 15, when registering a 
  printer, the default document size may not be the document size that matches 
  the language/region of the operating system. You can overcome this problem be
  registering the printer according to the following method.
  [Method 1]
   Specify "-P" instead of "-m" for the PPD file name option in the "lpadmin" command.
   If you specify "-P" for the PPD file, specify a fully-qualified path name or relative 
   path name for the specifed PPD file.
   Example)
   # /usr/sbin/lpadmin -p [printer name for registration] -P [PPD file path] -v 
   lpd:[device URI] -E
  [Method 2]
   1) Add "DefaultPaperSize Auto" to "/etc/cups/cupsd.conf", then restart CUPS.
   2) Register the printer.

- If you are using Debian GNU/Linux 4.0, you may take time to print from Evince 
  or may not be able to print some documents. To solve this problem, print the 
  document from other application.

- If you are using Debian GNU/Linux 4.0 r6, and attempt to print a text file 
  using the printer driver UI when EUC-JP is set as the locale, printing will fail.
  This problem can be solved by printing a PS command created with a text editor 
  or text/Postscript conversion program such as paps.

- If you are using Debian GNU/Linux 5.0.2, the gs-esp module is required to 
  install the common module.
  You can install the gs-esp module by executing the following command.
    # apt-get install gs-esp

- Depending on the distribution you are using, when you register the USB printer 
  with the print spooler specifying /dev/usb/lp* as [Device URI], printing may 
  fail with an error "Printer not Connected" displayed. To solve this problem, 
  specify the printer specific name as [Device URI] that is displayed by using 
  the following command.
    Example) When you use MF4600 Series
    1) Display the [Device URI]
      # /usr/sbin/lpinfo -v
      direct usb://Canon/MF4600%20Series%20(FAX)
      direct usb://Canon/MF4600%20Series%20(PCL5e)
      direct usb://Canon/MF4600%20Series%20(PCL6)
      direct usb://Canon/MF4600%20Series%20(UFRII%20LT)
    2) Register the printer
      #lpadmin -p MF4600_USB -m CNCUPSMF4600ZK.ppd 
                  -v usb://Canon/MF4600%20Series%20(UFRII%20LT) -E

- When printing PDF files from Adobe Reader in Mandriva, regardless of the 
  version being used, there may be instances where Ghostscript terminates 
  unexpectedly, causing the print queue to stop.
  This is caused by Ghostscript (8.60) not being able to analyze PS commands 
  created using PS files for which security settings have been specified, and 
  consequently terminating prematurely, thereby stopping the filtering process.
  This problem can be avoided by not printing PDF files that have security 
  settings using Adobe Reader.

- If you are using Mandriva One 2008 Spring or Mandriva 2008 PowerPack with CUPS 
  version 1.3.6, unintended print results may occur even when printing with 
  standard CUPS print functions.
  This problem can be solved by updating CUPS.

- If your version of Ghostscript is 8.6.x, you may not be able to print some 
  documents.

- There may be instances where MF4270, or L90/L160/L230 printers, depending on 
  the environment and type of print data, cannot print due to a printer data 
  error at the device end.
  This is caused by a time out error occurring at the device end.
  This problem can be avoided by turning the error time out setting off at the 
  device.

- If the Finisher Q3 or Saddle Finisher Q4 is attached and you specify A4, 
  Letter, or Legal as the page size, the document is not printed correctly when 
  you attempt to print with [Binding Location] set to [Short Edge (Top)] and 
  [Staple Position] set to [Top (Double)], or [Binding Location] set to 
  [Short Edge (Bottom)] and [Staple Position] set to [Bottom (Double)], in 
  the [Finishing] sheet.

- If you are using the iR C1021/C1021i, MF8400 Series, MF9100 Series, or 
  MF9300 Series, although IPv6 can be set from the control panel of the printer, 
  only IPv4 supports network connections.

- If you are using CentOS 5.3, you cannot print the number of copies as you 
  specified in Evince.
  You can solve this problem by printing from other PDF viewers such as 
  Adobe Reader or using the following methods.
  1) Set the number of copies to 1 and select a PS command for the output 
     destination in Evince to output a file.
  2) Print the PS command output as a file after specifying the number of 
     copies in cngplp.

- If you are using Ubuntu 9.04 and update the CUPS version to 
  "1.3.9-17ubuntu3.2", printing will fail due to improper PS data.
  You can avoid this problem by downgrading the CUPS version to 
  "1.3.9-17ubuntu3.1".
    - Execute the following command.
        # apt-get install cups=1.3.9-17ubuntu3.1

- If you are using Ubuntu 9.04, Ubuntu 9.10, Ubuntu 10.04, Ubuntu 10.10, Ubuntu
  11.04, Fedora 11, Fedora 12, Fedora 13, Fedora 14, or Fedora 15, and print 
  banner pages, the specified number of banner pages are printed.

- In Fedora 15 and Ubuntu 11.04, if you print with the brightness specified 
  between 0% and 9%, the brightness setting is not applied and the document is 
  printed the same as if the brightness was specified as 100%.

- If you are using the 32 bit or 64 bit version of Fedora 13, Fedora 14, or 
  Fedora 15, and print a TIFF or JPEG file from the driver UI or command line, 
  the printed image may be broken up. This problem can be solved by outputting 
  the file as a PostScript file from an application such as GIMP, then printing
  from the command line by typing the PostScript command used to output the file
  after [cngplp].

- If you are printing a TIFF or JPEG image from the driver UI or command line, the 
  image may be divided and printed across multiple pages.
  This problem can be solved by outputting the image as a PostScript file from an 
  application such as GIMP, then printing the PostScript file from the driver UI or 
  command line.

- If you are using Fedora 13, Fedora 14, or Fedora 15, even if you specify the 
  brightness and gamma settings from the driver UI or the command line, these 
  settings are not applied to the printed result from the second page onward. 
  This is due to these functions not being enabled because Ghostscript does not 
  correctly recognize the PostScript data created by the application.

- If you change the output method to [Print] after specifying [Secured Print] 
  as the output method and clicking [Save/Print Settings] in the driver UI, the 
  name of the document you output using [Secured Print] is displayed in the job 
  log of the printer. This problem can be solved by using the [Save/Print 
  Settings] button instead of the [Print] button to print when changing the 
  output method from [Secured Print] to [Print].

- After setting an entry or text view value, if you finish by clicking the [X] 
  button at the top right of the dialog box, the value is treated as a valid value 
  in the same way as when you click the [OK] button. If you want to disable the 
  set value, click the [Cancel] button.

- The following characters are treated differently in different CUPS versions, 
  therefore we recommend that you do not use them.
  "#"
  From CUPS version 1.4 onward, "#" is treated as a comment, therefore if you use 
  it in a command line, the characters following "#" are not recognized as commands.
  "\"
  If you enter "\\\123", the command is recognized in the following way, depending 
  on the version of CUPS:
  CUPS V1.1.22: \123
  CUPS V1.4.3: \\123

- Depending on the version of GTK (GIMP Toolkit), some characters may be 
  unintelligible when displayed on the screen, but this does not indicate 
  a problem with the functions and values set. Redraw the corresponding text 
  area to solve this problem.

- If you are using Fedora 12 or Ubuntu 9.10, when you change the default options 
  from the CUPS Web interface, the default values will be saved even if there is 
  a conflict between the settings for each function. Also, once the settings are 
  saved with a conflict, you cannot save the settings again even if you use the 
  Web interface to change them to the correct values where there is no conflict.
  If you display the [cngplp] dialog box in this situation, an invalid operation 
  may occur.
  You can use the following methods to solve this problem:
  [Method 1] Fedora 12 (32-bit/64-bit) and Ubuntu 9.10
   Re-register the printer that performed the invalid operation.
  [Method 2] Fedora 12 (32-bit/64-bit)
   Execute the following command to update CUPS:
    <For Fedora 12 (32-bit)> # yum update cups.i686
    <For Fedora 12 (64-bit)> # yum update cups.x86_64

- If you are using this driver in a 64-bit environment, and update a version 2.00, 
  2.10, or 2.20 driver to version 2.30 or later, errors may occur when printing.
  This problem can be solved by uninstalling (rpm -e [driver]) then re-installing 
  (rpm -i [driver]) the new driver.
  Alternatively, you can uninstall the old driver (rpm -e [driver]) instead of 
  updating it, then install the new driver (rpm -i [driver]).
  
- If you specify a document name that exceeds 32 bytes when printing, the character 
  string for the document name is deleted from the 33rd byte onward by the driver, 
  and the document name displayed in the device will be a maximum of 32 bytes.

- To use this printer driver, you need Ghostscript including common APIs.
  Before installing the driver, confirm that Ghostscript is installed. You can do 
  this using the following command in a terminal program such as GNOME Terminal.
    % gs -h | grep opvp
  If "opvp" and "oprp" are displayed in the result, Ghostscript including common 
  APIs is installed. If no result is displayed, you can obtain Ghostscript from 
  the following URL:
  http://opfc.sourceforge.jp/index.html.en

=================================================================================
Support
=================================================================================
This Software and Related Information are independently developed by Canon and 
distributed by your Canon local company. Canon (as a manufacturer of printers 
supporting this Software and Related Information) and your Canon local company
(as a distributor), will not respond to any inquiries about this Software or 
Related Information. However, any inquiries about printer repair, consumable 
supplies, and devices should be directed to your Canon local company.
=================================================================================
                                                      Copyright CANON INC. 2011
