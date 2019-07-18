# SP Nginx cache clear

Deze module maakt het in combinatie met een lua script voor nginx mogelijk om nginx caches te legen. Hiervoor maakt de module gebruik van rules. De module voegt een "Nginx cache clear" actie toe. Als je deze actie in een ruleset gebruikt, dan moet je één of meerdere reguliere expressies opgeven. De nginx caches van pagina's waarvan de paden matchen met de reguliere expressie worden dan gewist als die actie wordt getriggered.

Bijvoorbeeld een rule om de caches van nu pagina's te wissen, en het nieuws overzicht te verversen als nu pagina's worden bewerkt.
~~~
Event:
  - After updating existing content
  - After creating new content
Conditions:
  - Content is of type: nu
Action:
  - Nginx regex cache clear: 
      - [node:url]
      - nu.*
~~~

Voor www.sp.nl is een submodule toegevoegd met voorgedefinieerde rules.

## Example Nginx configuratie

[OpenResty](https://openresty.org/en/) kan gebruikt worden als nginx platform met lua support.

nginx.conf
~~~
worker_processes  auto;

pid        /var/run/nginx.pid;


events {
    worker_connections  1024;
}


http {
    include       mime.types;
    default_type  application/octet-stream;

    log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$http_x_forwarded_for"';

    sendfile        on;
    sendfile_max_chunk 1m;
    tcp_nopush      on;
    tcp_nodelay     on;

    open_file_cache          max=10000 inactive=30d;
    open_file_cache_valid    2m;
    open_file_cache_min_uses 1;
    open_file_cache_errors   on;

    keepalive_timeout  65;

    gzip  on;

    fastcgi_cache_path /var/cache/nginx/fastcgi-cache levels=1:2 keys_zone=nginx_fastcgi_cache:10m max_size=1024m inactive=24h;
    fastcgi_cache_key $http_host$request_uri$cookie_user;

    map $http_cookie $no_cache {
        default 0;
        ~SESS 1; # PHP session
    }

    include /etc/nginx/conf.d/*.conf;
}

~~~

vhost.conf
~~~
upstream php {
    server php:9000;
}

server {
    listen 80;

    root /var/www/html/build;
    index index.php;

    include fastcgi.conf;

    location / {

...
        if ($request_method = PURGE) {

            set $lua_purge_path "/var/cache/nginx/fastcgi-cache/";
            set $lua_purge_levels "1:2";
            set $lua_purge_upstream $http_host;

            content_by_lua_file purge.lua;
        }
...
    }
}
~~~

purge.lua:
~~~
-- Tit Petric, Monotek d.o.o., Tue 03 Jan 2017 06:54:56 PM CET
--
-- Delete nginx cached assets with a PURGE request against an endpoint
-- supports extended regular expression PURGE requests (/upload/.*)
--

function file_exists(name)
        local f = io.open(name, "r")
        if f~=nil then io.close(f) return true else return false end
end

function explode(d, p)
        local t, ll
        t={}
        ll=0
        if(#p == 1) then return {p} end
                while true do
                        l=string.find(p, d, ll, true) -- find the next d in the string
                        if l~=nil then -- if "not not" found then..
                                table.insert(t, string.sub(p, ll, l-1)) -- Save it in our array.
                                ll=l+1 -- save just after where we found it for searching next time.
                        else
                                table.insert(t, string.sub(p, ll)) -- Save what's left in our array.
                                break -- Break at end, as it should be, according to the lua manual.
                        end
                end
        return t
end

function purge(filename)
        -- ngx.log(ngx.STDERR, 'Purging: ' .. filename)
        if (file_exists(filename)) then
                -- ngx.say('Removing cache file: ' .. filename)
                os.remove(filename)
        end
end

function trim(s)
        return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

function exec(cmd)
        local handle = io.popen(cmd)
        local result = handle:read("*all")
        handle:close()
        return trim(result)
end

function list_files(cache_path, purge_upstream, purge_pattern)
        command = "/usr/bin/find " .. cache_path .. " -type f | /usr/bin/xargs --no-run-if-empty -n1000 /bin/grep -Eil -m 1 '^KEY: " .. purge_upstream .. purge_pattern .. "$' 2>&1"
        local result = exec(command)
        if result == "" then
                return {}
        end
        return explode("\n", result)
end

if ngx ~= nil then
        regex = ngx.unescape_uri(ngx.var.request_uri)

        ngx.say('Regex: ' .. regex)

        -- Clear proxy cache.
        local files = list_files(ngx.var.lua_purge_path, ngx.var.lua_purge_upstream, regex)
        for k, v in pairs(files) do
                hit = exec("/bin/grep -E '^KEY: " .. ngx.var.lua_purge_upstream .. regex .. "' " .. v)
                ngx.say('Cleared cache: ' .. hit)
                purge(v)
        end

        -- ngx.header.content_type = "text/plain"
        -- ngx.header["X-nginx-Purged-Count"] = table.getn(files)
        ngx.say("Ok")
        ngx.exit(ngx.OK)
end

~~~