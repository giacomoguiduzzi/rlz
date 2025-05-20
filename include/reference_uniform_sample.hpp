//
// Created by adrian on 13/11/18.
//

#ifndef RCT_REFERENCE_UNIFORM_SAMPLE_HPP
#define RCT_REFERENCE_UNIFORM_SAMPLE_HPP

#include <vector>
#include <unordered_map>
#include <sdsl/int_vector.hpp>

namespace rct {


    template<class t_value = uint32_t, uint64_t block_size_default = 1024>
    class reference_uniform_sample {

    public:
        static constexpr uint64_t default_block_size = block_size_default;
        typedef uint64_t size_type;
        typedef t_value value_type;
        typedef typename std::vector<value_type>::iterator iterator;
    private:
        std::vector<value_type> m_reference;

        void copy(const reference_uniform_sample &n){
            m_reference = n.m_reference;
        }

    public:

        reference_uniform_sample() = default;

        reference_uniform_sample(const std::vector<value_type> &container,
                                 const size_type reference_size, const size_type block_size = default_block_size,
                                 const double_t ratio = 0){

            auto ref_length = reference_size / sizeof(value_type);
            auto block_length = block_size / sizeof(value_type);
            auto num_samples = ref_length / block_length;
            auto input_size  = container.size();
            auto step_sample = input_size / num_samples;

            std::vector<value_type> reference;
            std::unordered_map<value_type, char> voc_reference;
            for(size_type i = 0; i < input_size; i += step_sample){
                for(size_type j = 0; j < block_length; ++j){
                    if(i + j >= input_size) break;
                    value_type value = container[i + j];
                    if(value == 0) std::cout << "Error" << std::endl;
                    if(voc_reference.count(value) == 0) voc_reference[value] = 1;
                    m_reference.push_back(value);
                }
            }

            for(size_type i = 0; i < input_size; ++i){
                value_type value = container[i];
                if(value == 0) std::cout << "Error" << std::endl;
                if(voc_reference.count(value) == 0){
                    voc_reference[value] = 1;
                    m_reference.push_back(value);
                }
            }


        }

        inline value_type operator[](const size_type& idx){
            return m_reference[idx];
        }

        inline value_type operator[](const size_type& idx) const{
            return m_reference[idx];
        }

        void* data(const size_type& idx){
            return &m_reference[idx];
        }

        inline size_type size() const{
            return m_reference.size();
        }

        const iterator begin()
        {
            return m_reference.begin();
        }

        //! Iterator that points to the element after the last element of int_vector.
        /*! Time complexity guaranty is O(1).
         */
        const iterator end()
        {
            return m_reference.end();
        }

        //! Copy constructor
        reference_uniform_sample(const reference_uniform_sample& o)
        {
            copy(o);
        }

        //! Move constructor
        reference_uniform_sample(reference_uniform_sample&& o)
        {
            *this = std::move(o);
        }

        void swap(reference_uniform_sample& r)
        {
            if (this != &r) {
                std::swap(m_reference, r.m_reference);
            }
        }

        reference_uniform_sample& operator=(const reference_uniform_sample& v)
        {
            if (this != &v) {
                copy(v);
            }
            return *this;
        }

        reference_uniform_sample& operator=(reference_uniform_sample&& v)
        {
            if (this != &v) {
                m_reference = std::move(v.m_reference);
            }
            return *this;
        }

        size_type serialize(std::ostream& out, sdsl::structure_tree_node* v=nullptr, std::string name="")const
        {
            sdsl::structure_tree_node* child = sdsl::structure_tree::add_child(v, name, sdsl::util::class_name(*this));
            size_type written_bytes = 0;
            sdsl::write_member(m_reference.size(), out, child, "size");
            written_bytes += sdsl::serialize_vector(m_reference, out, child, "reference");
            sdsl::structure_tree::add_size(child, written_bytes);
            return written_bytes;
        }

        void load(std::istream& in){
            size_type size;
            sdsl::read_member(size, in);
            m_reference.resize(size);
            sdsl::load_vector(m_reference, in);
        }

    };

}

#endif //RCT_REFERENCE_UNIFORM_SAMPLE_HPP
