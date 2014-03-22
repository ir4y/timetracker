-module(asteroid_stream_handler).

-export([init/4]).
-export([stream/3]).
-export([info/3]).
-export([terminate/2]).

-include_lib("celery/include/celery.hrl").

-record(state, {rpc_handlers}).

init(_Transport, Req, Opts, _Active) ->
	io:format("bullet init~n"),
    RpcHandlers = proplists:get_value(rpc_handlers, Opts),
	{ok, Req, #state{rpc_handlers=RpcHandlers}}.

stream(<<"ping">>, Req, State) ->
	{reply,
     jsx:encode([{<<"status">>, <<"pong">>}]),
     Req,
     State};
stream(Data, Req, State) ->
    Json = jsx:decode(Data),
    Function = proplists:get_value(<<"function">>, Json),
    Uuid = proplists:get_value(<<"uuid">>, Json),
    Arguments = proplists:get_value(<<"arguments">>, Json),

    Parent = self(),
    case Function of
        <<"subscribe_to">> -> subscriber:subscribe(lists:nth(1,Arguments), Uuid);
        <<"unsubscribe">> -> subscriber:unsubscribe(lists:nth(1,Arguments));
        _ -> erlang:spawn(fun() ->
                         Msg = #celery_msg{task=Function, args=Arguments},
                         Res = celery:call(Msg),
                         Parent ! {rpc_done, Uuid, jsx:to_json(
                                                     [{<<"status">>, Res#celery_res.status},
                                                      {<<"result">>, Res#celery_res.result}])}
                            end)
    end,
    {ok, Req, State}.

info({rpc_done, Uuid, Response}, Req, State) ->
    {reply,
     jsx:encode([{<<"uuid">>, Uuid}] ++ jsx:decode(Response)),
     Req,
     State};
info(Info, Req, State) ->
	io:format("info received ~p~n", [Info]),
	{ok, Req, State}.

terminate(_Req, _State) ->
	io:format("bullet terminate~n"),
	ok.
