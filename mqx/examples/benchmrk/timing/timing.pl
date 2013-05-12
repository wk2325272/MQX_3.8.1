if (@ARGV != 1)
{
    print_usage ();
    exit (0);
}
($f_in) = @ARGV;
$f_out = $f_in . ".html";
open(F_IN,$f_in) or die "Error opening input file '$f_in'!";
open(F_OUT,">",$f_out) or die "Error opening output file '$f_out'!";

$col=0;
$table=0;
html_start();
while($line = <F_IN>)
{
   chomp $line;

   # get rid of leading/trailing whitespace
   $line=trim($line);

   # skip if comment line
   next if ($line =~ /^#/ );

   $count = ($name,$value,$units,$loops,$loops_unit,$norm,$fluff)=split /,/, $line;

   $name=trim($name);
   $value=trim($value);
   $norm=trim($norm);
   next if (length($name) == 0);

   if ($count == 1) {
      html_group($name);
      $group=$name;
   } elsif ($group eq "Parameters") {
      html_param_value($name,$value);
   } elsif ($group eq "Kernel Options") {
      html_option_value($name,$value);
   } elsif ($group eq "Resource Usage") {
      html_resource_value($name,$value);
    } elsif ($count >= 4) {
      html_time_value($name,$value/$loops,$norm/$loops)
   }
}
html_end();
close F_IN;
close F_OUT;
print "File $f_out was created.\n";
exit (0);


sub html_start
{
    print F_OUT "<HTML>\r\n";
    print F_OUT "<HEAD>\r\n";
    print F_OUT "   <META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=iso-8859-1\">\r\n";
    print F_OUT "   <TITLE>MQX Timing Report</TITLE>\r\n";
    print F_OUT "<style type=\"text/css\">\r\n";
    print F_OUT "body{\r\n";
    print F_OUT " padding: 0px;\r\n";
    print F_OUT " margin: 30px;\r\n";
    print F_OUT " background-color: #FFF;\r\n";
    print F_OUT " font-family: Arial,Helvetica Neue, Helvetica,  sans-serif;\r\n";
    print F_OUT " font-size: 8pt;\r\n";
    print F_OUT " font-weight: normal;\r\n";
    print F_OUT " color: #51626F;\r\n";
    print F_OUT "}\r\n";
    print F_OUT ".reportname {\r\n";
    print F_OUT " font-size: 20pt;\r\n";
    print F_OUT " font-weight: bold;\r\n";
    print F_OUT "}\r\n";
    print F_OUT ".group {\r\n";
    print F_OUT " font-size: 10pt;\r\n";
    print F_OUT " font-weight: bold;\r\n";
    print F_OUT "}\r\n";
    print F_OUT ".row {\r\n";
    print F_OUT " font-size: 9pt;\r\n";
    print F_OUT "}\r\n";
    print F_OUT ".legalese {\r\n";
    print F_OUT " font-size: 7pt;\r\n";
    print F_OUT "}\r\n";
    print F_OUT "</style>\r\n";
    print F_OUT "</HEAD>\r\n";
    print F_OUT "<BODY>\r\n";
    print F_OUT "<TABLE CELLSPACING=0 CELLPADDING=0 WIDTH=\"1000\" >\r\n";
    print F_OUT "<TR><TD WIDTH=\"100%\" class=\"reportname\">\r\n";
    print F_OUT "<CENTER>MQX Timing Report\r\n";
    print F_OUT "</CENTER></TD></TR></TABLE>\r\n";
}


sub html_group 
{
    print F_OUT "</TR>\r\n" if ($col==1) ;
    print F_OUT "</TABLE>\r\n" if ($table==1) ;
    $col=0;
    $table=0;
    print F_OUT "<TABLE CELLSPACING=0 CELLPADDING=0 WIDTH=\"1000\" ><TR>\r\n";
    print F_OUT "<TD WIDTH=\"100%\ class=\"group\"><HR><B>", $_[0], "</B></TD>\r\n";
    print F_OUT "</TABLE></TR>\r\n"
}


sub html_table_entry
{
    if ($table==0) {
       print F_OUT "<TABLE CELLSPACING=0 CELLPADDING=0 WIDTH=\"1000\" >\r\n";
       $table=1;
    }
    print F_OUT "<TR>\r\n" if ($col==0) ;
    print F_OUT "<TD WIDTH=\"",  $_[1],"%\" class=\"row\">", $_[0],  "</TD>\r\n";
    print F_OUT "<TD WIDTH=\"",  $_[3],"%\" class=\"row\">", $_[2], "</TD>\r\n";
    if ($col==1) {
       print F_OUT "</TR>\r\n";
       $col=0;
    } else {
       $col=1;
    }
}


sub html_param_value
{
   if ($col==0) {
      html_table_entry($_[0], "15", $_[1], "35");
   } else {
      html_table_entry($_[0], "30", $_[1], "20");
   }
}


sub html_option_value
{
    if ($_[1] eq "0") {
       html_table_entry($_[0], "30", "No", "20");
    } else {
       html_table_entry($_[0], "30", "Yes", "20");
    }
}


sub html_resource_value
{
   html_table_entry($_[0], "30", $_[1], "20");
}


sub html_time_value
{
	if (1000 > $_[1]) {
		$value = sprintf "<TABLE CLASS=\"row\"><TR><TD WIDTH=\"100\"><i>%6.2fns</i></TD>", $_[1];
	} else {
		$value = sprintf "<TABLE CLASS=\"row\"><TR><TD WIDTH=\"100\"><i>%6.2fus</i></TD>", $_[1] / 1000;
	}

	if (1000000 > $_[2]) {
		$value = sprintf "%s <TD WIDTH=\"100\"><i>%6.2fus 1MHz</i></TD></TR></TABLE>", $value, $_[2] / 1000;
	} else {
		$value = sprintf "%s <TD WIDTH=\"100\"><i>%6.2fms 1MHz</i></TD></TR></TABLE>", $value, $_[2] / 1000000;
	}

	html_table_entry($_[0], "30", $value, "20");
}


sub html_end
{
    print F_OUT "</TR>\r\n" if ($col==1);
    print F_OUT "</TABLE>\r\n" if ($table==1);
    print F_OUT "<TABLE CELLSPACING=0 CELLPADDING=0 WIDTH=\"1000\" ><TR><TD class=\"legalese\">\r\n";

    print F_OUT "<HR><BR>(c) 2009 Freescale Semiconductor. All rights reserved.\r\n";
    print F_OUT "Confidential Information. No part of this document may be\r\n";
    print F_OUT "copied or reproduced in any form or by any means without prior written\r\n";
    print F_OUT "consent of Freescale Semiconductor. Freescale MQX is a trademark\r\n";
    print F_OUT "of Freescale Semiconductor. All other trademarks are property\r\n";
    print F_OUT "of their respective owners.\r\n";
    print F_OUT "</TD></TR></TABLE></BODY></HTML>\r\n";
}


sub trim($)
{
   my $string = shift;
   $string =~ s/^\s+//;
   $string =~ s/\s+$//;
   return $string;
}


sub print_usage
{
    my $readme =
        "\nFreescale Semiconductor MQX(TM) Timing script\n\n".
        "\tThis script adds HTML tags to make human readable\n".
        "\tHTML form of the timing benchmark console output.\n";

    print "$readme\n";
    print "Usage:\n";
    print "timing.exe <timing_benchmark_console_output_filename>\n\n";
    print "Example:\ntiming.exe test.log\n\n";
}

