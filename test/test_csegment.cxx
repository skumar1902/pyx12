// Boost.Test
#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;

#include "segment.hxx"

// BOOST_WARN_MESSAGE, BOOST_CHECK_MESSAGE
// BOOST_CHECK_THROW, BOOST_CHECK_NO_THROW
// BOOST_CHECK_EQUAL
// BOOST_CHECK_PREDICATE, BOOST_CHECK_EQUAL_COLLECTIONS
// BOOST_MESSAGE
// BOOST_CHECKPOINT


//////////////////////////////////////////////////////////////////////////////
// Test Identity Transforms
//////////////////////////////////////////////////////////////////////////////
void Identity1()
{
    std::string seg_str("TST*AA*1*Y*BB:5*ZZ");
    Pyx12::segment seg_data(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(seg_data.format(), seg_str + '~'); 
}

void Identity2()
{
    std::string seg_str("ISA*00*          *00*          *ZZ*ZZ000          *");
    seg_str += "ZZ*ZZ001          *030828*1128*U*00401*000010121*0*T*:";
    seg_str += '\n';
    Pyx12::segment seg_data(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.format() == seg_str + '~'); 
}

void Identity3()
{
    std::string seg_str("TST*AA*1*Y*BB:5*ZZ~");
    Pyx12::segment seg_data(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.format() == seg_str); 
}

//////////////////////////////////////////////////////////////////////////////
// Test Arbitrary Delimiters
//////////////////////////////////////////////////////////////////////////////
void ArbitraryIdentity()
{
    std::string seg_str("TST&AA!1!1&BB!5&ZZ");
    Pyx12::segment seg_data(seg_str, '+', '&', '!');
    BOOST_CHECK(seg_data.format() == seg_str + '+'); 
}

void ArbitraryGetSegID()
{
    std::string seg_str("TST&AA!1!1&BB!5&ZZ");
    Pyx12::segment seg_data(seg_str, '+', '&', '!');
    BOOST_CHECK(seg_data.get_seg_id() == "TST"); 
}

void ArbitraryLength()
{
    std::string seg_str("TST&AA!1!1&BB!5&ZZ");
    Pyx12::segment seg_data(seg_str, '+', '&', '!');
    BOOST_CHECK(seg_data.length() == 3); 
}

void ArbitraryGetItem3()
{
    std::string seg_str("TST&AA!1!1&BB!5&ZZ");
    Pyx12::segment seg_data(seg_str, '+', '&', '!');
    BOOST_CHECK(seg_data.get_value("TST03") == "ZZ"); 
}

void ArbitraryGetItem1()
{
    std::string seg_str("TST&AA!1!1&BB!5&ZZ");
    Pyx12::segment seg_data(seg_str, '+', '&', '!');
    BOOST_CHECK(seg_data.format("TST01") == "AA!1!1"); 
}

void ArbitraryOtherTerms()
{
    std::string seg_str("TST&AA!1!1&BB!5&ZZ");
    Pyx12::segment seg_data(seg_str, '+', '&', '!');
    BOOST_CHECK(seg_data.format('~', '*', ':') == "TST*AA:1:1*BB:5*ZZ~");
}


//////////////////////////////////////////////////////////////////////////////
// Test Alter
//////////////////////////////////////////////////////////////////////////////
void AlterElement() {
    seg_str = "TST*AA:1:1*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    seg_data.set("TST03", "YY");
    BOOST_CHECK_EQUAL(seg_data.format(), "TST*AA:1:1*BB:5*YY~");
}

void AlterExtendElementBlank() {
    seg_str = "TST*AA:1:1*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    seg_data.set("TST05", "");
    BOOST_CHECK_EQUAL(seg_data.format(), "TST*AA:1:1*BB:5*ZZ~");
}

void AlterExtendElement() {
    seg_str = "TST*AA:1:1*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    seg_data.set("TST05", "AR");
    BOOST_CHECK_EQUAL(seg_data.format(), "TST*AA:1:1*BB:5*ZZ**AR~");
}

void AlterComposite() {
    seg_str = "TST*AA:1:1*BB:5*ZZ~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    seg_data.set("TST02", "CC:2");
    BOOST_CHECK_EQUAL(seg_data.format(), "TST*AA:1:1*CC:2*ZZ~");
}

void AlterExtendComposite() {
    seg_str = "TST*AA:1:1*BB:5*ZZ~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    seg_data.set("TST02-4", "T");
    BOOST_CHECK_EQUAL(seg_data.format(), "TST*AA:1:1*BB:5::T*ZZ~");
}

void AlterExtendComposite2() {
    seg_str = "TST*AA:1:1*BB:5*ZZ~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    seg_data.set("TST05-2", "T");
    BOOST_CHECK_EQUAL(seg_data.format(), "TST*AA:1:1*BB:5*ZZ**:T~");
}

void AlterExtendCompositeBlank1() {
    seg_str = "TST*AA:1:1*BB:5*ZZ~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    seg_data.set("TST02-4", "");
    BOOST_CHECK_EQUAL(seg_data.format(), "TST*AA:1:1*BB:5*ZZ~");
}

void AlterExtendCompositeBlank2() {
    seg_str = "TST*AA:1:1*BB:5*ZZ~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    seg_data.set("TST05-4", "");
    BOOST_CHECK_EQUAL(seg_data.format(), "TST*AA:1:1*BB:5*ZZ~");
}


//////////////////////////////////////////////////////////////////////////////
// Test Composite
//////////////////////////////////////////////////////////////////////////////
void CompositeIsA() {
    seg_str = "TST*AA:1:1*BB:5*ZZ~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.is_composite("TST01"));
    BOOST_CHECK(!seg_data.is_element("TST01"));
}
    
void CompositeLength() {
    seg_str = "TST*AA:1:1*BB:5*ZZ~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(len(seg_data.get("01")), 3);
}

void CompositeIndexing() {
    seg_str = "TST*AA:1:1*BB:5*ZZ~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(seg_data.get_value("TST01-1"), "AA");
    BOOST_CHECK_EQUAL(seg_data.get_value("TST01-3"), "Y");
    BOOST_CHECK_EQUAL(seg_data.get_value("TST01-4"), NULL);
}


//////////////////////////////////////////////////////////////////////////////
// Test Simple
//////////////////////////////////////////////////////////////////////////////
void SimpleIsA() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.is_element("TST01"));
    BOOST_CHECK(!seg_data.is_composite("TST01"));
}
    
void SimpleLength() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(len(seg_data.get("01")), 1);
}

void SimpleIndexing() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(seg_data.get_value("TST01"), "AA");
    BOOST_CHECK_EQUAL(seg_data.get_value("TST02"), "1");
    BOOST_CHECK_EQUAL(seg_data.get_value("TST03"), "Y");
    BOOST_CHECK_EQUAL(seg_data.get_value("TST05"), "ZZ");
    BOOST_CHECK_EQUAL(seg_data.get_value("TST06"), NULL);
}

void SimpleSpaces() {
    seg_str = "TST*AA*      *BB~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(len(seg_data.get_value("TST02")), 10);
    BOOST_CHECK_EQUAL(seg_data.get_value("TST02"), "      ");
}

//////////////////////////////////////////////////////////////////////////////
// Test Get Value
//////////////////////////////////////////////////////////////////////////////
void getElementValueOK() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(seg_data.get_value("TST01"), seg_data.get("TST01").format());
    BOOST_CHECK_EQUAL(seg_data.get_value("TST04"), seg_data.get("TST04").format());
    BOOST_CHECK_EQUAL(seg_data.get_value("TST03"), seg_data.get("TST03").format());
    BOOST_CHECK_EQUAL(seg_data.get_value("TST05"), seg_data.get("TST05").format());
    BOOST_CHECK_EQUAL(seg_data.get_value("TST06"), seg_data.get("TST06").format());
}
    
void getCompositeValueOK() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(seg_data.get_value("TST04-1"), seg_data.get("TST04-1").format());
    BOOST_CHECK_EQUAL(seg_data.get_value("TST04-2"), seg_data.get("TST04-2").format());
}


//////////////////////////////////////////////////////////////////////////////
// Test RefDes
//////////////////////////////////////////////////////////////////////////////
void RefDesSimple1() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.get_value("TST01"), "AA");
    BOOST_CHECK(seg_data.get_value("01"), "AA");
}

void RefDesFailSegId() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECKRaises(EngineError, seg_data.get_value, "XXX01");
}

void RefDesSimple2() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.get_value("TST02"), "1");
    BOOST_CHECK(seg_data.get_value("02"), "1");
}

void RefDesComposite1() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.get_value("TST04-2"), "5");
    BOOST_CHECK(seg_data.get_value("04-2"), "5");
}

void RefDesComposite2() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.get_value("TST04-1"), "BB");
    BOOST_CHECK(seg_data.get_value("04-1"), "BB");
}

void RefDesComposite3() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.get_value("TST04"), "BB:5");
    BOOST_CHECK(seg_data.get_value("04"), "BB:5");
}

void RefDesNone() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.get_value("TST15"), NULL);
    BOOST_CHECK(seg_data.get_value("15"), NULL);
    BOOST_CHECK(seg_data.get_value("TST15-2"), NULL);
    BOOST_CHECK(seg_data.get_value("15-2"), NULL);
}


//////////////////////////////////////////////////////////////////////////////
// Test Empty
//////////////////////////////////////////////////////////////////////////////
void EmptySeg() {
    seg_str = "AAA";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.is_empty());
}

void EmptySegBad1() {
    seg_str = "AAA*1~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(!seg_data.is_empty());
}

void EmptySegBad2() {
    seg_str = "AAA*:1~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(!seg_data.is_empty());
}

void EmptyComp1() {
    comp_str = "";
    comp = Pyx12::composite(comp_str, ':');
    BOOST_CHECK(comp.is_empty());
}

void EmptyComp2() {
    comp_str = "::";
    comp = Pyx12::composite(comp_str, ':');
    BOOST_CHECK(comp.is_empty());
}

void EmptyCompBad1() {
    comp_str = "1::a";
    comp = Pyx12::composite(comp_str, ':');
    BOOST_CHECK(!comp.is_empty());
}

void EmptyCompBad2() {
    comp_str = "::a";
    comp = Pyx12::composite(comp_str, ':');
    BOOST_CHECK(!comp.is_empty());
}

void EmptyCompBad3() {
    comp_str = "a";
    comp = Pyx12::composite(comp_str, ':');
    BOOST_CHECK(!comp.is_empty());
}


//////////////////////////////////////////////////////////////////////////////
// Test Indexing
//////////////////////////////////////////////////////////////////////////////
void IndexSimple_1() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(seg_data.get_value("TST01"), "AA");
}

void IndexSimple_2() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(seg_data.get_value("TST01-1"), "AA");
}

void IndexComposite_1() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(seg_data.get_value("TST04-1"), "BB");
}

void IndexComposite_2() {
    seg_str = "TST*AA*1*Y*BB:5*ZZ";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK_EQUAL(seg_data.get_value("TST04-2"), "5");
}
            

//////////////////////////////////////////////////////////////////////////////
// Test Is Valid Seg ID
//////////////////////////////////////////////////////////////////////////////
void ValidSegId() {
    seg_str = "AAA";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(seg_data.is_seg_id_valid());
}

void SegIDEmptySeg() {
    seg_str = "";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(!seg_data.is_seg_id_valid());
}

void SegIdTooLong() {
    seg_str = "AAAA*1~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(!seg_data.is_seg_id_valid());
}

void SegIdTooShort() {
    seg_str = "A*1~";
    seg_data = Pyx12::segment(seg_str, '~', '*', ':');
    BOOST_CHECK(!seg_data.is_seg_id_valid());
}

test_suite*
init_unit_test_suite( int, char* [] ) {
    test_suite* test = BOOST_TEST_SUITE( "Segment unit tests" );

    // this example will pass cause we know ahead of time number of expected failures
    test->add( BOOST_TEST_CASE( &Identity1), 0);
    test->add( BOOST_TEST_CASE( &Identity2), 0);
    test->add( BOOST_TEST_CASE( &Identity3), 0);
    test->add( BOOST_TEST_CASE( &ArbitraryIdentity), 0);
    test->add( BOOST_TEST_CASE( &ArbitraryGetSegID), 0);
    test->add( BOOST_TEST_CASE( &ArbitraryLength), 0);
    test->add( BOOST_TEST_CASE( &ArbitraryGetItem3), 0);
    test->add( BOOST_TEST_CASE( &ArbitraryGetItem1), 0);
    test->add( BOOST_TEST_CASE( &ArbitraryOtherTerms), 0);

    // Alter
    test->add( BOOST_TEST_CASE( &AlterElement), 0);
    test->add( BOOST_TEST_CASE( &AlterExtendElementBlank), 0);
    test->add( BOOST_TEST_CASE( &AlterExtendElement), 0);
    test->add( BOOST_TEST_CASE( &AlterComposite), 0);
    test->add( BOOST_TEST_CASE( &AlterExtendComposite), 0);
    test->add( BOOST_TEST_CASE( &AlterExtendComposite2), 0);
    test->add( BOOST_TEST_CASE( &AlterExtendCompositeBlank1), 0);
    test->add( BOOST_TEST_CASE( &AlterExtendCompositeBlank2), 0);

    // Composite
    test->add( BOOST_TEST_CASE( &CompositeIsA), 0);
    test->add( BOOST_TEST_CASE( &CompositeLength), 0);
    test->add( BOOST_TEST_CASE( &CompositeIndexing), 0);

    // Simple
    test->add( BOOST_TEST_CASE( &SimpleIsA), 0);
    test->add( BOOST_TEST_CASE( &SimpleLength), 0);
    test->add( BOOST_TEST_CASE( &SimpleIndexing), 0);
    test->add( BOOST_TEST_CASE( &SimpleSpaces), 0);
    test->add( BOOST_TEST_CASE( &getElementValueOK), 0);
    test->add( BOOST_TEST_CASE( &getCompositeValueOK), 0);
    
    // RefDes
    test->add( BOOST_TEST_CASE( &RefDesSimple1), 0);
    test->add( BOOST_TEST_CASE( &RefDesFailSegId), 0);
    test->add( BOOST_TEST_CASE( &RefDesSimple2), 0);
    test->add( BOOST_TEST_CASE( &RefDesComposite1), 0);
    test->add( BOOST_TEST_CASE( &RefDesComposite2), 0);
    test->add( BOOST_TEST_CASE( &RefDesComposite3), 0);
    test->add( BOOST_TEST_CASE( &RefDesNone), 0);

    // Empty
    test->add( BOOST_TEST_CASE( &EmptySeg), 0);
    test->add( BOOST_TEST_CASE( &EmptySegBad1), 0);
    test->add( BOOST_TEST_CASE( &EmptySegBad2), 0);
    test->add( BOOST_TEST_CASE( &EmptyComp1), 0);
    test->add( BOOST_TEST_CASE( &EmptyComp2), 0);
    test->add( BOOST_TEST_CASE( &EmptyCompBad1), 0);
    test->add( BOOST_TEST_CASE( &EmptyCompBad2), 0);
    test->add( BOOST_TEST_CASE( &EmptyCompBad3), 0);

    // Indexing
    test->add( BOOST_TEST_CASE( &IndexSimple_1), 0);
    test->add( BOOST_TEST_CASE( &IndexSimple_2), 0);
    test->add( BOOST_TEST_CASE( &IndexComposite_1), 0);
    test->add( BOOST_TEST_CASE( &IndexComposite_2), 0);
    test->add( BOOST_TEST_CASE( &ValidSegId), 0);

    // Seg ID
    test->add( BOOST_TEST_CASE( &SegIDEmptySeg), 0);
    test->add( BOOST_TEST_CASE( &SegIdTooLong), 0);
    test->add( BOOST_TEST_CASE( &SegIdTooShort), 0);

    return test;
}

