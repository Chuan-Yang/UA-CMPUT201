#!/usr/bin/perl

# get the directories, and make the tmp directory
$#ARGV >= 0 or die "ERROR: test_???.pl submission_tar_file";
$submission_file = $ARGV[ 0 ];
$exec_name = "draw1b";
$pub_dir = "~burch/pub/a1b";

# files we want to see / don't want to see in submission tar
@good_files = ( "Makefile", "README", "$exec_name.c", "$exec_name.h" );
@bad_files = ( $exec_name, "$exec_name.o", "core", "Sketchpad.jar" );

# files we want to copy from $pub_dir
@copy_files = ( "Sketchpad.jar" );

# test files in $pub_dir and sample output (must be in same order)
@test_files = ( "inputfile.1", "inputfile.2", "inputfile.3" );
@output_files = ( "outputfile.1", "outputfile.2", "outputfile.3" );


# extract
! -d "test_${exec_name}_submission" or die "test_${exec_name}_submission/ directory already exists, check it, then remove it";
mkdir( "test_${exec_name}_submission" ) or die "couldn't create test_${exec_name}_submission/ directory";
if( system( "tar xvf \"$submission_file\" -C test_${exec_name}_submission" ) != 0 ) {
    rmdir( "test_${exec_name}_submission" );
    die "failed to extract submission tar file";
}
chdir( "test_${exec_name}_submission" ) or die "couldn't enter test_${exec_name}_submission/ directory";

# check submission contents
foreach $i ( @good_files ) {
    -f $i or die "submission tar does not include $i (no directories in submit.tar)\n";
}
foreach $i ( @bad_files ) {
    ! -f $i or die "submission tar should not include $i\n";
}

# clean
print "running make clean\n";
system( "make clean" ) == 0 or die "make clean returned errors";

# build
print "running make\n";
system( "make" ) == 0 or die "make returned errors";
-x $exec_name or die "couldn't find $exec_name executable after make";

# copy in necessary files
foreach $i ( @copy_files ) {
    system( "cp $pub_dir/$i ." ) == 0 or die "could not copy $pub_dir/$i";
}

# run tests
for( $i = 0; $i < @test_files; ++$i ) {
    print "testing sample $test_files[ $i ]\n";
    system( "./$exec_name $pub_dir/$test_files[ $i ] > $output_files[ $i ]" ) == 0 or die "error running $exec_name $pub_dir/$test_files[ $i ]";
    system( "tail -n +3 $output_files[ $i ] | diff -b $pub_dir/$output_files[ $i ] -" ) == 0 or die "output does not match for $test_files[ $i ]";
}

# clean-up
chdir( ".." ) or die "couldn't exit test_${exec_name}_submission/ directory";
system( "rm -rf test_${exec_name}_submission" ) == 0 or die "could not remove test_${exec_name}_submission_directory";
