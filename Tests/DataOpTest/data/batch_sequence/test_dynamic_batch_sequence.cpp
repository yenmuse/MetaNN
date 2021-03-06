#include <MetaNN/meta_nn.h>
#include <calculate_tags.h>
#include <iostream>
using namespace std;
using namespace MetaNN;

namespace
{
    void test_batch_scalar_sequence_case1()
    {
        cout << "Test dynamic batch scalar sequence case 1...\t";
        using TItem = ScalarSequence<CheckElement, CheckDevice>;
        
        static_assert(IsBatchScalarSequence<DynamicBatch<TItem>>);
        static_assert(IsBatchScalarSequence<DynamicBatch<TItem>&>);
        static_assert(IsBatchScalarSequence<DynamicBatch<TItem>&&>);
        static_assert(IsBatchScalarSequence<const DynamicBatch<TItem>&>);
        static_assert(IsBatchScalarSequence<const DynamicBatch<TItem>&&>);

        DynamicBatch<TItem> check;
        assert(check.Shape().SeqLenContainer().empty());

        // check contains 4 sequences, with length = 13, 1, 100, 87
        int c = 0;
        TItem item1(13);
        for (size_t i = 0; i < 13; ++i) item1.SetValue(i, (float)(c++));
        
        TItem item2(1);
        for (size_t i = 0; i < 1; ++i) item2.SetValue(i, (float)(c++));
        
        TItem item3(100);
        for (size_t i = 0; i < 100; ++i) item3.SetValue(i, (float)(c++));
        
        TItem item4(87);
        for (size_t i = 0; i < 87; ++i) item4.SetValue(i, (float)(c++));
        
        check.PushBack(item1); check.PushBack(item2); check.PushBack(item3); check.PushBack(item4);
        assert(check.Shape().SeqLenContainer()[0] == 13);
        assert(check.Shape().SeqLenContainer()[1] == 1);
        assert(check.Shape().SeqLenContainer()[2] == 100);
        assert(check.Shape().SeqLenContainer()[3] == 87);

        auto cm = Evaluate(check);

        const auto& seqCont = cm.Shape().SeqLenContainer();
        assert(seqCont.size() == 4);
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < seqCont[i]; ++j)
            {
                assert(cm[i][j] == check[i][j]);
            }
        }
        cout << "done" << endl;
    }
    
    void test_batch_matrix_sequence_case1()
    {
        cout << "Test dynamic batch matrix sequence case 1...\t";
        using TItem = MatrixSequence<CheckElement, CheckDevice>;
        
        static_assert(IsBatchMatrixSequence<DynamicBatch<TItem>>);
        static_assert(IsBatchMatrixSequence<DynamicBatch<TItem> &>);
        static_assert(IsBatchMatrixSequence<DynamicBatch<TItem> &&>);
        static_assert(IsBatchMatrixSequence<const DynamicBatch<TItem> &>);
        static_assert(IsBatchMatrixSequence<const DynamicBatch<TItem> &&>);

        DynamicBatch<TItem> check(13, 35);
        assert(check.Shape().SeqLenContainer().empty());
        
        // check contains 4 sequences, with length = 13, 1, 100, 87
        int c = 0;
        TItem item1(13, 13, 35);
        TItem item2(1, 13, 35);
        TItem item3(100, 13, 35);
        TItem item4(87, 13, 35);
        for (size_t i = 0; i < 13; ++i)
        {
            for (size_t j = 0; j < 35; ++j)
            {
                for (size_t k = 0; k < 100; ++k)
                {
                    if (k < 13)  item1.SetValue(k, i, j, (CheckElement)(c++));
                    if (k < 1)   item2.SetValue(k, i, j, (CheckElement)(c++));
                    if (k < 100) item3.SetValue(k, i, j, (CheckElement)(c++));
                    if (k < 87)  item4.SetValue(k, i, j, (CheckElement)(c++));
                }
            }
        }
        
        check.PushBack(item1); check.PushBack(item2); check.PushBack(item3); check.PushBack(item4);
        assert(check.Shape().SeqLenContainer()[0] == 13);
        assert(check.Shape().SeqLenContainer()[1] == 1);
        assert(check.Shape().SeqLenContainer()[2] == 100);
        assert(check.Shape().SeqLenContainer()[3] == 87);

        auto cm = Evaluate(check);
        
        const auto& seqCont = cm.Shape().SeqLenContainer();
        assert(seqCont.size() == 4);
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < seqCont[i]; ++j)
            {
                for (size_t k = 0; k < 13; ++k)
                {
                    for (size_t l = 0; l < 35; ++l)
                    {
                        assert(cm[i][j](k, l) == check[i][j](k, l));
                    }
                }
            }
        }
        cout << "done" << endl;
    }
    
   
    void test_batch_3d_array_sequence_case1()
    {
        cout << "Test dynamic batch 3d array sequence case 1...\t";
        using TItem = ThreeDArraySequence<CheckElement, CheckDevice>;
        
        static_assert(IsBatchThreeDArraySequence<DynamicBatch<TItem>>);
        static_assert(IsBatchThreeDArraySequence<DynamicBatch<TItem> &>);
        static_assert(IsBatchThreeDArraySequence<DynamicBatch<TItem> &&>);
        static_assert(IsBatchThreeDArraySequence<const DynamicBatch<TItem> &>);
        static_assert(IsBatchThreeDArraySequence<const DynamicBatch<TItem> &&>);

        DynamicBatch<TItem> check(7, 13, 35);
        assert(check.Shape().SeqLenContainer().empty());
        
        int c = 0;
        TItem item1(13, 7, 13, 35);
        TItem item2(1, 7, 13, 35);
        TItem item3(100, 7, 13, 35);
        TItem item4(87, 7, 13, 35);
        for (size_t p = 0; p < 7; ++p)
        {
            for (size_t i = 0; i < 13; ++i)
            {
                for (size_t j = 0; j < 35; ++j)
                {
                    for (size_t k = 0; k < 100; ++k)
                    {
                        if (k < 13)  item1.SetValue(k, p, i, j, (CheckElement)(c++));
                        if (k < 1)   item2.SetValue(k, p, i, j, (CheckElement)(c++));
                        if (k < 100) item3.SetValue(k, p, i, j, (CheckElement)(c++));
                        if (k < 87)  item4.SetValue(k, p, i, j, (CheckElement)(c++));
                    }
                }
            }
        }
        check.PushBack(item1); check.PushBack(item2); check.PushBack(item3); check.PushBack(item4);
        assert(check.Shape().SeqLenContainer()[0] == 13);
        assert(check.Shape().SeqLenContainer()[1] == 1);
        assert(check.Shape().SeqLenContainer()[2] == 100);
        assert(check.Shape().SeqLenContainer()[3] == 87);
        
        auto cm = Evaluate(check);
        
        const auto& seqCont = cm.Shape().SeqLenContainer();
        assert(seqCont.size() == 4);
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < seqCont[i]; ++j)
            {
                for (size_t p = 0; p < 7; ++p)
                {
                    for (size_t k = 0; k < 13; ++k)
                    {
                        for (size_t l = 0; l < 35; ++l)
                        {
                            assert(cm[i][j](p, k, l) == check[i][j](p, k, l));
                        }
                    }
                }
            }
        }

        cout << "done" << endl;
    }
}

namespace Test::Data::BatchSequence
{
    void test_dynamic_batch_sequence()
    {
        test_batch_scalar_sequence_case1();
        test_batch_matrix_sequence_case1();
        test_batch_3d_array_sequence_case1();
    }
}