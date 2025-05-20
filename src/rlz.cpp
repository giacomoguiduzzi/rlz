//
// Created by adrian on 14/11/18.
//

#include <vector>
#include <random>
#include <functional>
#include <rlz_naive.hpp>

int main(int argc, const char* argv[]) {

    typedef uint64_t value_type;
    typedef rct::rlz_naive<value_type , rct::reference_uniform_sample<value_type >,
            sdsl::csa_bitcompressed<sdsl::int_alphabet<>>> rlz_type;
    typedef typename rlz_type::factor_type factor_type;

    if(argc == 4){

        std::string input = argv[1];
        uint64_t size_ref = (value_type) atoi(argv[2]) * 1024 * 1024;
        uint64_t size_block = (value_type) atoi(argv[3]);
        auto size = util::file::file_size(input) / sizeof(value_type);
        std::vector<value_type > movements(size);
        std::ifstream in(input);
        in.read((char*) movements.data(), movements.size()*sizeof(value_type));
        in.close();

        for(auto &m : movements){
            if(m == 0){
                std::cout << "Movements contains 0" << std::endl;
                exit(10);
            }
        }

        rlz_type m_rlz(movements, size_ref, size_block);
        m_rlz.init_factorization(&movements);

        std::vector<typename rct::rlz_csa_bc_int64::factor_type> factors;
        while (m_rlz.has_next()) {
            auto f = m_rlz.next();
            //factors_log << "f.length: " << f.length << " f.offset: "  << f.offset << std::endl;
            factors.push_back(f);
        }
        std::vector<uint64_t> result;
        std::cout << "Decompressing: " << std::flush;
        m_rlz.decompress(factors, result);
        if(result.size() != movements.size()){
            std::cout << "Error size" << std::endl;
            exit(1);
        }
        for(uint64_t i = 0; i < result.size(); ++i){
            if(result[i] != movements[i]){
                std::cout << "Error movement: " << i << std::endl;
                exit(1);
            }
        }
        std::cout << "Phrases: " << factors.size() << std::endl;

    }

}