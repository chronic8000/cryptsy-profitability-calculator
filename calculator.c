var net = require('net');
var Cryptsy = require('cryptsy');
var cryptsy = new Cryptsy('YOUR-KEY', 'YOUR-SECRET');

var destinations = [
    ['10.0.0.251', 3334], // Worldcoin
    ['10.0.0.251', 3335], // Grandcoin
    ['10.0.0.251', 3336], // Mooncoin
    ['10.0.0.251', 3337], // Netcoin
    ['10.0.0.251', 3338], // Fastcoin
]

var s = net.Server(function(client_socket) {
    var i = Math.floor(Math.random() * destinations.length);
    console.log("connecting to " + destinations[i].toString() + "\n");
    var dest_socket = net.Socket();
    dest_socket.connect(destinations[i][1], destinations[i][0]);

    dest_socket.on('data', function(d) {
        client_socket.write(d);
    });
    client_socket.on('data', function(d) {
        dest_socket.write(d);
    });
});
s.listen(3333);

// Calculates number of coins made in a day
// Assume 3 MH/s
// Yeild is the number of coins per block
// Difficulty is quite obvious
function calcNumCoin(yeild, difficulty) {
        return (yeild/difficulty)*(3000000*86400/2^32);
        // 86400 seconds in day
}

function calcProfitability(yeild, difficulty, mktID) {
        cryptsy.api('singlemarketdata', {marketid: mktID}, function(err, data) {
                if (err) {
                        throw err;
                } else {
                        // TODO - parse json data ('data' variable) for response
                        return data.return[0][0]lasttradeprice * calcNumCoin(yeild, difficulty);
                }
        });
}
