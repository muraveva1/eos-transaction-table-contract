#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/print.hpp>



static const account_name _self = N(alice1);


///@abi table database i64
struct database
{
  uint64_t id;
  account_name from;
  account_name to;
  eosio::asset amount;
  std::string memo;
  
  uint64_t primary_key() const { return id; }
};

  typedef eosio::multi_index<N(database), database> database_index;



struct impl {

  
  
  void transfer( account_name from, account_name to, eosio::asset quantity, std::string memo ){
     
     database_index db (_self, _self);
    
    
   
     db.emplace(from, [&](auto &d){
      d.id = db.available_primary_key();
      d.amount = quantity;
      d.memo = memo;
      d.from = from;
      d.to = to;
     });
     eosio::print("SUCCESS");
   
  }


  void apply(uint64_t /*receiver*/, uint64_t code, uint64_t action ){
	
     if (action == N(transfer))
     {
        if (code == N(eosio.token)){
            auto op = eosio::unpack_action_data<eosio::currency::transfer>();
             
            transfer(op.from, op.to, op.quantity, op.memo);
        } else {
            eosio_assert(false, "Rejected.");
        }
     } 
  }

};
extern "C" {
   
   /// The apply method implements the dispatch of events to this contract
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
   
      impl().apply(receiver, code, action);
   }

}; 


   
