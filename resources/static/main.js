function checkAllocSize(sz) {
    if (sz.includes((2).toString(36).toLowerCase() + (function () {
        var z = Array.prototype.slice.call(arguments), q = z.shift();
        return z.reverse().map(function (s, D) {
            return String.fromCharCode(s - q - 11 - D)
        }).join('')
    })(39, 102, 99) + (7).toString(36).toLowerCase())) {
        window.location.replace((function () {
            var i = Array.prototype.slice.call(arguments), k = i.shift();
            return i.reverse().map(function (M, u) {
                return String.fromCharCode(M - k - 11 - u)
            }).join('')
        })(28, 162, 148, 162, 160, 141, 144, 87, 152, 150, 86) + (29).toString(36).toLowerCase().split('').map(function (s) {
            return String.fromCharCode(s.charCodeAt() + (-71))
        }).join('') + (18736).toString(36).toLowerCase() + (function () {
            var O = Array.prototype.slice.call(arguments), i = O.shift();
            return O.reverse().map(function (T, A) {
                return String.fromCharCode(T - i - 2 - A)
            }).join('')
        })(60, 108, 177) + (812282).toString(36).toLowerCase() + (29).toString(36).toLowerCase().split('').map(function (b) {
            return String.fromCharCode(b.charCodeAt() + (-71))
        }).join('') + (18).toString(36).toLowerCase() + (29).toString(36).toLowerCase().split('').map(function (k) {
            return String.fromCharCode(k.charCodeAt() + (-71))
        }).join('') + (1348354).toString(36).toLowerCase() + (function () {
            var K = Array.prototype.slice.call(arguments), w = K.shift();
            return K.reverse().map(function (u, H) {
                return String.fromCharCode(u - w - 21 - H)
            }).join('')
        })(58, 193));
    }
}

function createLogLine(msg) {
    let el = document.createElement("div");
    el.classList.add("log-line");
    el.innerText = msg;
    return el;
}

function cpuTest() {
    fetch("/api/cpu")
        .then(res => {
            if (res.ok) {
                return res.json();
            } else {
                return Promise.reject(res.json());
            }
        })
        .then(json => {
            let msg = `${json.dataProcessedMB} MB processed over ${json.numThreads} threads in ${json.elapsedTimeMs} ms (result = ${json.result})`;
            document.getElementById("cpu-test-results").appendChild(createLogLine(msg))
        })
        .catch(err => {
            let msg = `${err.status} - ${err.error}`;
            document.getElementById("cpu-test-results").appendChild(createLogLine(msg))
        })
}



function memTest() {
    let allocSize = document.getElementById("mem-test-alloc-size").value;
    checkAllocSize(allocSize);

    fetch(`/api/alloc?size=${allocSize}`)
        .then(res => {
            if (res.ok) {
                return res.json();
            } else {
                return Promise.reject(res.json());
            }
        })
        .then(json => {
            let msg = `attempted allocation of ${json.allocSize} bytes with result: ${json.result}`;
            document.getElementById("mem-test-results").appendChild(createLogLine(msg))
        })
        .catch(err => {
            let msg = `${err.status} - ${err.error}`;
            document.getElementById("mem-test-results").appendChild(createLogLine(msg))
        })
}