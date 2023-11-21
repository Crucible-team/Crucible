
if not Script_Initialized(script_pid()) then

	backlog_post("health charger int: " .. tostring(GetEntity()))

	table.insert(EVENT_BUS, GetEntity())

	usefunc = function(reciever)
		if EVENT_BUS[GetEntity()].fuel <= 0 and EVENT_BUS[GetEntity()].bCanUse == 1 then
			 EVENT_BUS[GetEntity()].ACTIVE = 0
			 EVENT_BUS[GetEntity()].bCanUse = 0
		end

		EVENT_BUS[GetEntity()].ACTIVE = 1
		if EVENT_BUS[GetEntity()].ACTIVE == 1 then

-- 			backlog_post("health charger active: " .. tostring(EVENT_BUS[GetEntity()].ACTIVE))
			if reciever ~= INVALID_ENTITY then
				backlog_post("health charger reciever: " .. tostring(reciever))

				if EVENT_BUS[GetEntity()].FirstOn ~= 1
					EVENT_BUS[GetEntity()].FirstOn = EVENT_BUS[GetEntity()].FirstOn + 1

					--Play first time on sound here

					EVENT_BUS[GetEntity()].playsoundtime = 0.60 + getDeltaTime()

				end

				local health = GetGlobalScene().Component_GetHealth(reciever)

-- 				backlog_post("health charger reciever: " .. tostring(health))
				if health ~= nil then

					if health.GetHealth() < health.GetMaxHealth() then

						if EVENT_BUS[GetEntity()].givenexthp <= getDeltaTime() then

							if EVENT_BUS[GetEntity()].FirstOn == 1 and EVENT_BUS[GetEntity()].playsoundtime <= getDeltaTime() then

								EVENT_BUS[GetEntity()].FirstOn = EVENT_BUS[GetEntity()].FirstOn + 1

								-- play looping charging sound here

							end

							EVENT_BUS[GetEntity()].fuel = EVENT_BUS[GetEntity()].fuel - 1
							health.AddHealth(1)
							backlog_post("health changed: " .. tostring(health.GetHealth()))
							EVENT_BUS[GetEntity()].getnexthp = 0.1 + getDeltaTime()

						end

					end
				end
			end

		end

		EVENT_BUS[GetEntity()].ACTIVE = 0
	end

	EVENT_BUS[GetEntity()] = { bCanUse = 1,fuel = 65, ACTIVE = 0,FirstOn = 0, RECIEVER = INVALID_ENTITY, givenexthp = 0.0,playsoundtime = 0.0, use = usefunc  }

end




