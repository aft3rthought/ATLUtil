//  Copyright © 2019 Maxwell Robinson. All rights reserved.

#pragma once

namespace atl
{
    template <typename iterator_type>
    struct container_range
    {
        iterator_type begin_itr;
        iterator_type end_itr;
        
        container_range(iterator_type in_begin_itr, iterator_type in_end_itr)
        :
        begin_itr(in_begin_itr),
        end_itr(in_end_itr)
        {}
        
        iterator_type begin() const { return begin_itr; }
        iterator_type end() const { return end_itr; }
    };
    
    template <typename iterator_type>
    container_range<iterator_type> make_range(iterator_type begin_itr, iterator_type end_itr) { return container_range<iterator_type>(begin_itr, end_itr); }

    template <typename iterator_type>
    container_range<iterator_type> make_range_n(iterator_type begin_itr, size_t length) { return container_range<iterator_type>(begin_itr, begin_itr + length); }
}
